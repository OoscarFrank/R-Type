#pragma once

#include <vector>
#include <iostream>
#include <string>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #include <windows.h>
#endif

namespace TypesLitterals {
    constexpr char operator""_c(unsigned long long c) { return static_cast<char>(c); }
    constexpr short operator""_s(unsigned long long c) { return static_cast<short>(c); }
    constexpr int operator""_i(unsigned long long c) { return static_cast<int>(c); }
    constexpr long operator""_l(unsigned long long c) { return static_cast<long>(c); }
    constexpr u_char operator""_uc(unsigned long long c) { return static_cast<u_char>(c); }
    constexpr u_short operator""_us(unsigned long long c) { return static_cast<u_short>(c); }
    constexpr u_int operator""_ui(unsigned long long c) { return static_cast<u_int>(c); }
    constexpr u_long operator""_ul(unsigned long long c) { return static_cast<u_long>(c); }
}

class Stream
{
private:
    std::vector<unsigned char> _buffer;

public:
    /**
     * @brief Default constructor for creating an empty Stream.
     */
    Stream();

    /**
     * @brief Copy constructor for creating a Stream from another Stream.
     *
     * @param stream The Stream object to copy from.
     */
    Stream(const Stream &stream);

    /**
     * @brief Constructor for creating a Stream from a vector of unsigned char.
     *
     * @param buffer The vector of unsigned char to initialize the Stream with.
     */
    Stream(const std::vector<unsigned char> &buffer);

    /**
     * @brief Constructor for creating a Stream from a string.
     *
     * @param str The string to initialize the Stream with.
     */
    Stream(const std::string &str);

    /**
     * @brief Constructor for creating a Stream from a char array with a specified size.
     *
     * @param str The char array to initialize the Stream with.
     * @param size The size of the char array.
     */
    Stream(const char str[], size_t size);

    /**
     * @brief Destructor for cleaning up resources associated with the Stream.
     */
    ~Stream();

    /**
     * @brief Get a constant reference to the internal buffer.
     *
     * @return A constant reference to the internal buffer containing byte data.
     */
    const std::vector<unsigned char> &getBuffer() const;

    /**
     * @brief Overloaded addition assignment operator for appending another Stream to this Stream.
     *
     * @param stream The Stream object to append.
     */
    void operator += (const Stream &stream);

    /**
     * @brief Overloaded addition assignment operator for appending a single byte to this Stream.
     *
     * @param data The byte to append.
     */
    void operator += (const unsigned char &data);

    /**
     * @brief Overloaded addition assignment operator for appending a string to this Stream.
     *
     * @param str The string to append.
     */
    void operator += (const std::string &str);

    /**
     * @brief Overloaded assignment operator for copying the content of another Stream.
     *
     * @param stream The Stream object to copy from.
     * @return A reference to this Stream after the assignment.
     */
    Stream operator = (const Stream &stream);

    /**
     * @brief Get the value of a byte at the specified index in the Stream.
     *
     * @param index The index of the byte to retrieve.
     * @return The byte value at the specified index.
     */
    unsigned char operator[](size_t index) const;

    /**
     * @brief Get a reference to a byte at the specified index in the Stream.
     *
     * @param index The index of the byte to retrieve.
     * @return A reference to the byte at the specified index.
     */
    unsigned char &operator[](size_t index);

    /**
     * @brief Set the data of the Stream using a char array with a specified size.
     *
     * @param data The char array to set as the Stream's data.
     * @param size The size of the char array.
     */
    void setDataCharArray(const char *data, size_t size);


    /**
     * @brief Append an unsigned long value to the Stream.
     *
     * @param data The unsigned long value to append to the Stream.
     */
    void setDataULong(unsigned long data);

    /**
     * @brief Append an unsigned integer value to the Stream.
     *
     * @param data The unsigned integer value to append to the Stream.
     */
    void setDataUInt(unsigned int data);

    /**
     * @brief Append an unsigned short value to the Stream.
     *
     * @param data The unsigned short value to append to the Stream.
     */
    void setDataUShort(unsigned short data);

    /**
     * @brief Append an unsigned char value to the Stream.
     *
     * @param data The unsigned char value to append to the Stream.
     */
    void setDataUChar(unsigned char data);

    /**
     * @brief Append a long value to the Stream.
     *
     * @param data The long value to append to the Stream.
     */
    void setDataLong(long data);

    /**
     * @brief Append an integer value to the Stream.
     *
     * @param data The integer value to append to the Stream.
     */
    void setDataInt(int data);

    /**
     * @brief Append a short value to the Stream.
     *
     * @param data The short value to append to the Stream.
     */
    void setDataShort(short data);

    /**
     * @brief Append a char value to the Stream.
     *
     * @param data The char value to append to the Stream.
     */
    void setDataChar(char data);

        /**
     * @brief Append the contents of another Stream to this Stream.
     *
     * @param stream The Stream object whose contents to append to this Stream.
     * @return A reference to this Stream after the operation.
     */
    Stream &operator<<(const Stream &stream);

    /**
     * @brief Append an unsigned char (u_char) value to this Stream.
     *
     * @param data The unsigned char value to append to this Stream.
     * @return A reference to this Stream after the operation.
     */
    Stream &operator<<(u_char data);

    /**
     * @brief Append an unsigned short (u_short) value to this Stream.
     *
     * @param data The unsigned short value to append to this Stream.
     * @return A reference to this Stream after the operation.
     */
    Stream &operator<<(u_short data);

    /**
     * @brief Append an unsigned integer (u_int) value to this Stream.
     *
     * @param data The unsigned integer value to append to this Stream.
     * @return A reference to this Stream after the operation.
     */
    Stream &operator<<(u_int data);

    /**
     * @brief Append an unsigned long (u_long) value to this Stream.
     *
     * @param data The unsigned long value to append to this Stream.
     * @return A reference to this Stream after the operation.
     */
    Stream &operator<<(u_long data);

    /**
     * @brief Append a char (char) value to this Stream.
     *
     * @param data The char value to append to this Stream.
     * @return A reference to this Stream after the operation.
     */
    Stream &operator<<(char data);

    /**
     * @brief Append a short (short) value to this Stream.
     *
     * @param data The short value to append to this Stream.
     * @return A reference to this Stream after the operation.
     */
    Stream &operator<<(short data);

    /**
     * @brief Append an integer (int) value to this Stream.
     *
     * @param data The integer value to append to this Stream.
     * @return A reference to this Stream after the operation.
     */
    Stream &operator<<(int data);

    /**
     * @brief Append a long (long) value to this Stream.
     *
     * @param data The long value to append to this Stream.
     * @return A reference to this Stream after the operation.
     */
    Stream &operator<<(long data);

    /**
     * @brief Append a boolean (bool) value to this Stream.
     *
     * @param data The boolean value to append to this Stream.
     * @return A reference to this Stream after the operation.
     */
    Stream &operator<<(bool data);


    /**
     * @brief Get the current size of the Stream.
     *
     * @return The number of bytes in the Stream.
     */
    size_t size() const;

    /**
     * @brief Clear the contents of the Stream.
     *
     * This method removes all data from the Stream, making it empty.
     */
    void clear();


    /**
     * @brief Extract an unsigned long value from the Stream.
     *
     * @return The unsigned long value extracted from the Stream.
     */
    unsigned long getDataULong();

    /**
     * @brief Extract an unsigned integer value from the Stream.
     *
     * @return The unsigned integer value extracted from the Stream.
     */
    unsigned int getDataUInt();

    /**
     * @brief Extract an unsigned short value from the Stream.
     *
     * @return The unsigned short value extracted from the Stream.
     */
    unsigned short getDataUShort();

    /**
     * @brief Extract an unsigned char value from the Stream.
     *
     * @return The unsigned char value extracted from the Stream.
     */
    unsigned char getDataUChar();

    /**
     * @brief Extract a long value from the Stream.
     *
     * @return The long value extracted from the Stream.
     */
    long getDataLong();

    /**
     * @brief Extract an integer value from the Stream.
     *
     * @return The integer value extracted from the Stream.
     */
    int getDataInt();

    /**
     * @brief Extract a short value from the Stream.
     *
     * @return The short value extracted from the Stream.
     */
    short getDataShort();

    /**
     * @brief Extract a char value from the Stream.
     *
     * @return The char value extracted from the Stream.
     */
    char getDataChar();


        /**
     * @brief Extract an unsigned char (u_char) value from the Stream and store it in the provided variable.
     *
     * @param data Reference to a u_char variable where the extracted value will be stored.
     * @return A reference to this Stream after the operation.
     */
    Stream &operator>>(u_char &data);

    /**
     * @brief Extract an unsigned short (u_short) value from the Stream and store it in the provided variable.
     *
     * @param data Reference to a u_short variable where the extracted value will be stored.
     * @return A reference to this Stream after the operation.
     */
    Stream &operator>>(u_short &data);

    /**
     * @brief Extract an unsigned integer (u_int) value from the Stream and store it in the provided variable.
     *
     * @param data Reference to a u_int variable where the extracted value will be stored.
     * @return A reference to this Stream after the operation.
     */
    Stream &operator>>(u_int &data);

    /**
     * @brief Extract an unsigned long (u_long) value from the Stream and store it in the provided variable.
     *
     * @param data Reference to a u_long variable where the extracted value will be stored.
     * @return A reference to this Stream after the operation.
     */
    Stream &operator>>(u_long &data);

    /**
     * @brief Extract a char (char) value from the Stream and store it in the provided variable.
     *
     * @param data Reference to a char variable where the extracted value will be stored.
     * @return A reference to this Stream after the operation.
     */
    Stream &operator>>(char &data);

    /**
     * @brief Extract a short (short) value from the Stream and store it in the provided variable.
     *
     * @param data Reference to a short variable where the extracted value will be stored.
     * @return A reference to this Stream after the operation.
     */
    Stream &operator>>(short &data);

    /**
     * @brief Extract an integer (int) value from the Stream and store it in the provided variable.
     *
     * @param data Reference to an int variable where the extracted value will be stored.
     * @return A reference to this Stream after the operation.
     */
    Stream &operator>>(int &data);

    /**
     * @brief Extract a long (long) value from the Stream and store it in the provided variable.
     *
     * @param data Reference to a long variable where the extracted value will be stored.
     * @return A reference to this Stream after the operation.
     */
    Stream &operator>>(long &data);

    /**
     * @brief Extract a boolean (bool) value from the Stream and store it in the provided variable.
     *
     * @param data Reference to a bool variable where the extracted value will be stored.
     * @return A reference to this Stream after the operation.
     */
    Stream &operator>>(bool &data);


    /**
     * @brief Create and return a substream starting from the specified position to the end of the Stream.
     *
     * @param pos The position within the Stream where the substream will start.
     * @return A new Stream containing the portion of data from the specified position to the end of the original Stream.
     */
    Stream subStream(size_t pos) const;

    /**
     * @brief Create and return a substream starting from the specified position with the specified size.
     *
     * @param start The starting position within the Stream for the substream.
     * @param size The size of the substream to be created.
     * @return A new Stream containing the specified portion of data from the original Stream.
     */
    Stream subStream(size_t start, size_t size) const;

    /**
     * @brief Convert the contents of the Stream to a string.
     *
     * @return A string representation of the data in the Stream.
     */
    std::string toString() const;

    class SubStreamError : public std::exception
    {
    public:
        std::string _message;
        const char *what() const noexcept override { return _message.c_str(); }
    };
};

std::ostream &operator<<(std::ostream &os,const Stream &stream);
