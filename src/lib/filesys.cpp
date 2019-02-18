#include "filesys.hpp"
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <stdexcept>
#include <streambuf>
#include "../settings.hpp"

namespace PhysFS {
    /*****************************************************************/
    /*                              fbuf                             */
    /*****************************************************************/

    class fbuf : public std::streambuf {
    private:
        fbuf (const fbuf & other);
        fbuf& operator=(const fbuf& other);

        int_type underflow (void) {
            if (PHYSFS_eof(file)) {
                return traits_type::eof();
            }
            size_t bytesRead = PHYSFS_read(file, buffer, 1, bufferSize);
            if (bytesRead < 1) {
                return traits_type::eof();
            }
            setg(buffer, buffer, buffer + bytesRead);
            return (unsigned char) *gptr();
        }

        pos_type seekoff (off_type pos, std::ios_base::seekdir dir, std::ios_base::openmode mode) {
            switch (dir) {
            case std::ios_base::beg:
                PHYSFS_seek(file, pos);
                break;
            case std::ios_base::cur:
                // subtract characters currently in buffer from seek position
                PHYSFS_seek(file, (PHYSFS_tell(file) + pos) - (egptr() - gptr()));
                break;
            case std::ios_base::end:
                PHYSFS_seek(file, PHYSFS_fileLength(file) + pos);
                break;
            }
            if (mode & std::ios_base::in) {
                setg(egptr(), egptr(), egptr());
            }
            if (mode & std::ios_base::out) {
                setp(buffer, buffer);
            }
            return PHYSFS_tell(file);
        }

        pos_type seekpos (pos_type pos, std::ios_base::openmode mode) {
            PHYSFS_seek(file, pos);
            if (mode & std::ios_base::in) {
                setg(egptr(), egptr(), egptr());
            }
            if (mode & std::ios_base::out) {
                setp(buffer, buffer);
            }
            return PHYSFS_tell(file);
        }

        int_type overflow (int_type c = traits_type::eof()) {
            if (pptr() == pbase() && c == traits_type::eof()) {
                return 0; // no-op
            }
            if (PHYSFS_write(file, pbase(), pptr() - pbase(), 1) < 1) {
                return traits_type::eof();
            }
            if (c != traits_type::eof()) {
                if (PHYSFS_write(file, &c, 1, 1) < 1) {
                    return traits_type::eof();
                }
            }

            return 0;
        }

        int sync (void) {
            return overflow();
        }

        char* buffer;
        size_t const bufferSize;
    protected:
        PHYSFS_File* const file;
    public:
        fbuf (PHYSFS_File * file, std::size_t bufferSize = 2048) : file(file), bufferSize(bufferSize) {
            buffer = new char[bufferSize];
            char * end = buffer + bufferSize;
            setg(end, end, end);
            setp(buffer, end);
        }

        ~fbuf (void) {
            sync();
            delete [] buffer;
        }
    };

    /*****************************************************************/
    /*                         base_fstream                          */
    /*****************************************************************/

    base_fstream::base_fstream (PHYSFS_File* file) : file(file) {
        if (file == NULL) {
            throw std::invalid_argument("attempted to construct fstream with NULL ptr");
        }
    }

    base_fstream::~base_fstream (void) {
        PHYSFS_close(file);
    }

    size_t base_fstream::length() {
        return PHYSFS_fileLength(file);
    }

    PHYSFS_File* openWithMode (char const* filename, mode openMode) {
        PHYSFS_File* file = NULL;
        switch (openMode) {
        case WRITE:
            file = PHYSFS_openWrite(filename);
            break;
        case APPEND:
            file = PHYSFS_openAppend(filename);
            break;
        case READ:
            file = PHYSFS_openRead(filename);
        }
        if (file == NULL) {
            throw std::invalid_argument("file not found: " + std::string(filename));
        }
        return file;
    }

    /*****************************************************************/
    /*                           ifstream                            */
    /*****************************************************************/

    ifstream::ifstream (const std::string& filename) : base_fstream(openWithMode(filename.c_str(), READ)), std::istream(new fbuf(file)) {}

    ifstream::~ifstream (void) {
        delete rdbuf();
    }
}

namespace YUNIK_GTC {
    /*****************************************************************/
    /*                            Filesys                            */
    /*****************************************************************/

    void Filesys_init (char* argv0) {
        if (!PHYSFS_init(argv0)) {
            std::cerr << "ERROR: " << PHYSFS_getLastError() << std::endl;
            exit(EXIT_FAILURE);
        }

        /* Mount resource files */
        char path_delimiter = '_';
        std::string path_delimiter_str(1, path_delimiter);
        for (const auto& entry : std::filesystem::directory_iterator(YUNIK_GTC_RESOURCE_DIR)) {
            std::filesystem::path path = entry.path();
            if (path.extension().compare(YUNIK_GTC_RESOURCE_EXTENSION) != 0) {
                continue;
            }

            std::string path_unix = path.string();
            std::replace(path_unix.begin(), path_unix.end(), '\\', '/');

            std::string path_origin = path.stem().string();
            auto start = path_origin.find(path_delimiter_str);
            if (start == std::string::npos) {
                PHYSFS_mount((const char*)path_unix.c_str(), "/", 1);
            } else {
                auto end = path_origin.length() - start;
                std::string path_rel = path_origin.substr(start, end);
                std::replace(path_rel.begin(), path_rel.end(), path_delimiter, '/');

                PHYSFS_mount((const char*)path_unix.c_str(), (const char*)path_rel.c_str(), 1);
            }
        }
    }

    void Filesys_deinit (void) {
        PHYSFS_deinit();
    }

    /*****************************************************************/
    /*                          ArchiveFile                          */
    /*****************************************************************/

    ArchiveFile::ArchiveFile (const char* aFilePath) {
        if (PHYSFS_exists(aFilePath) == 0) {
            std::cerr << "Error: '" << aFilePath << "' not found" << std::endl;
            err = ArchiveFileError::FILE_NOT_FOUND;
        }
        mFileHandle = PHYSFS_openRead(aFilePath);
        if (!mFileHandle) {
            std::cerr << "Error: '" << aFilePath << "' load failed" << std::endl;
            err = ArchiveFileError::FILE_LOAD_FAILED;
        }
        err = ArchiveFileError::FILE_NO_ERROR;
    }

    ArchiveFile::~ArchiveFile (void) {
        if (mFileHandle) {
            PHYSFS_close(mFileHandle);
        }
    }

    int ArchiveFile::eof (void) {
        return PHYSFS_eof(mFileHandle);
    }

    int ArchiveFile::read (void* aDst, unsigned int aBytes) {
        return (int) PHYSFS_read(mFileHandle, aDst, aBytes, 1);
    }

    int ArchiveFile::readBytes (void* aDst, unsigned int len) {
        return (int) PHYSFS_readBytes(mFileHandle, aDst, len);
    }

    int ArchiveFile::length (void) {
        return (int) PHYSFS_fileLength(mFileHandle);
    }

    int ArchiveFile::seek (int aOffset) {
        PHYSFS_seek(mFileHandle, aOffset);
    }

    int ArchiveFile::tell (void) {
        return (int) PHYSFS_tell(mFileHandle);
    }

    ArchiveFileError ArchiveFile::failure (void) {
        return err;
    }

    /*****************************************************************/
    /*                           ifstream                            */
    /*****************************************************************/

    ifstream::ifstream (const std::string& filename) : PhysFS::ifstream(filename) {}

    ifstream::~ifstream (void) {}
}
