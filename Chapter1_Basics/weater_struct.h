#pragma once

#pragma pack(1)
struct WeaterReport {
    char zipcode[8];
    size_t temp;
    uint32_t postcode;
    char type;
};
#pragma pack()
