#pragma once

#include <string>
#include <rpc.h>

// https://stackoverflow.com/a/24365878

static std::string generate_uuid_string() {
    UUID uuid;
    UuidCreate(&uuid);

    unsigned char* str;
    UuidToStringA(&uuid, &str);

    std::string uuidString(reinterpret_cast<const char*>(str));
    
    RpcStringFreeA(&str);
    return uuidString;
}


