# trim

TRIM a device in FreeBSD.

## Usage

    trim [device]

## Example

    -# hexdump -C /dev/ada3
    00000000  ce 5e 98 86 b6 67 28 48  e2 66 86 ec e1 e5 6f 0a  |.^...g(H.f....o.|
    ...
    -# trim ada3
    trim 256060514304 bytes from ada3? (yes/no) yes
    -# hexdump -C /dev/ada3
    00000000  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
    *

## Notes

Use of this software may result in data loss. Hopefully.

It is *not* a tool for TRIMing free space in a filesystem like Linux's `fstrim`.

It is *not* a secure erase tool.  For that use `camcontrol(1)`:

    camcontrol security [device] -s erase -e erase
