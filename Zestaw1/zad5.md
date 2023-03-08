<h1>Zadanie 5</h1>
<p>W dokumentacji POSIX API znajdź opisy czterech podstawowych funkcji plikowego wejścia-wyjścia, tzn. open, read, write i close. Czy zgadzają się one z tym, co pamiętasz z przedmiotu „Systemy operacyjne”? Jakie znaczenie ma wartość 0 zwrócona jako wynik funkcji read?</p>
<p>--------------------------------------------------------------------------------------------------------</p>
<h2>Funkcja open</h2>
``
	   #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>

       int open(const char *pathname, int flags);
       int open(const char *pathname, int flags, mode_t mode);
``
<h3>Description</h3>
<p>The open() system call opens the file specified by pathname. If the specified file does nto exist it may 
optionally be created by open() (if O_CREAT is specified in flags).

The return value of open() is a file descriptor, a small, nonnegative integer that is used in subsequent system calls to refer to the open file. The file descirptor returned by a successful call will be the lowest-numbered file descriptor not currently open for the process.

The argument flags must include one of the following access modes: 
*O_RDONLY - read-only
*O_WRONLY - write-only
*O_RDWR - read/write 
</p>
<h3>Return value</h3>
<p>Function return the new file descriptor(a nonnegative integer) or -1 if an error occurred, in which case
errno is set appropriately.</p>
<p>--------------------------------------------------------------------------------------------------------</p>
<h2>Funkcja read</h2>
``
	#include <unistd.h>

	       ssize_t read(int fd, void *buf, size_t count);  
``
ssize_t - signed integer data type
<h3>Description</h3>
<p>read() attempts to read up to count bytes from file descriptor fd, into the buffer starting at buf.
On files that support seeking, the read operation commences at the file offset, and the file offset is incremented by the
number of bytes read. If the file offset is at or past the end of file, no bytes are read, and read() return zero.

If count is zero, read() may detect the errors. In the absence of any errors, or if read() does not check for errors, a read() with a count of 0 returns zero and has no other effects.</p>
<h3>Return value</h3>
<p>On success, the number of bytes read is returned(zero indicates end of file), and the file position is advanced by this number. It is not an error if this number is smaller than the number of bytes requested; this may happen for because fewer bytes are actually available right now.

On error, -1 is returned and errno is set appropriately. In this case, it is left unspecified whether the file position changes.</p>
<p>--------------------------------------------------------------------------------------------------------</p>
<h2>Funkcja write</h2>
``
	#include <unistd.h>

       ssize_t write(int fd, const void *buf, size_t count);
``
<h3>Description</h3>
<p>write() writes up to count bytes from the buffer starting at buf to the file referred to by the file descriptor fd.
The number of bytes written may be less than count if, for example, there is insufficient space on the underlying physical medium.
For a seekable file wirting takes place at the file offset, and the file offset is incremented by the number of bytes actually written. If the file was opened with O_APPEND, the file offset is first set to the end of the file before writing.The adjustment of the file offset and the write operation are performed as an atomic step.</p>
<h3>Return value</h3>
<p>On success, the number of bytes written is returned. On error, -1 is returned, and errno is set to indicate the cause of the error.
If count is zero and fd refers to a regular file, the write() may return a failure status if one of the errors is detected. If no errors are detected, or error detection is not perfomed, 0 will be returned without causing any other effect. If count is zero and fd refers to a file other than a regular file, the results are not specified.</p>
<p>--------------------------------------------------------------------------------------------------------</p>
<h2>Funkcja close</h2>
``
#include <unistd.h>

       int close(int fd);
``
<h3>Description</h3>
<p>close() closes a file descriptor, so that it no longer refers to any file and may be reused.</p>
<h3>Return value</h3>
<p>close() returns zero on success. On error, -1 is returned, and errno is set appropriately.</p>