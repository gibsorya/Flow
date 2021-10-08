/* 
*
* This file was based off of Godot's error_list.h file.
*
*/
#ifndef ERROR_LIST
#define ERROR_LIST

/* 

This is an error list for organizing different error types. List will expand as certain error types become more frequent.

*/
enum Error {
    SUCCESS, // (0)
    FAILED, // you messed up
    ERR_CANT_CREATE, //Cannot create [object]. Typically used for Vulkan objects if VkResult != VK_SUCCESS
    ERR_INVALID,
};

#endif