#ifndef HEADER_ENCRYPTION_MANAGER
#define HEADER_ENCRYPTION_MANAGER

#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

/**
 * Operates over all files in the directory, recursively, decrypting or encrypting depending on mode.
*/
void operate_dir(const char* dir_name, int indent, int mode);

/**
 * Insecure reversible encryption on a file
 */ 
int toy_encrypt(const char* file);

/**
 * Insecure reversible decryption on a file
 */ 
int toy_decrypt(const char* file);

#endif