#include "../../include/Socket.h"
#include <assert.h>
#include <algorithm>
#include <unistd.h>

namespace Server
{
    Socket::Socket()
        : descriptor(-1)
    {
    }

    Socket::Socket(int fd)
        : descriptor(fd)
    {
    }

    Socket::Socket(Socket&& other)
        : descriptor(other.release())
    {
    }

    Socket::~Socket()
    {
        close();
    }

    Socket& Socket::operator=(Socket&& other)
    {
        descriptor = other.release();
        return *this;
    }

    Socket::operator int() const
    {
        return descriptor;
    }

    void Socket::close()
    {
        if (descriptor != -1)
            ::close(descriptor);
        descriptor = -1;
    }

    void Socket::reset(int descriptor)
    {
        close();
        this->descriptor = descriptor;
    }

    int Socket::release()
    {
        int oldDescriptor = -1;
        std::swap(oldDescriptor, descriptor);
        return oldDescriptor;
    }

    Pipe::Pipe()
    {
        int fds[2];
        assert(pipe(fds) != -1);
        read.reset(fds[0]);
        write.reset(fds[1]);
    }

    void Pipe::close()
    {
        read.close();
        write.close();
    }
}