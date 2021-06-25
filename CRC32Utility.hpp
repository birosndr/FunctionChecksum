#ifndef __CRC32UTILITY_HPP__
#define __CRC32UTILITY_HPP__
#pragma once

unsigned reverse (unsigned x);

unsigned int crc32a (unsigned char* message);

unsigned int crc32b (unsigned char* message);

unsigned int crc32c (unsigned char* message);


#endif // !__CRC32UTILITY_HPP__