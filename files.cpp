#include "pxt.h"
#include "MicroBitFile.h"
#include "MicroBitFileSystem.h"

using namespace pxt;

/**
* File seek offset modifier
*/
enum FileSystemSeekFlags {
    //% block=set
    Set = MB_SEEK_SET,
    //% block=current
    Current = MB_SEEK_CUR,
    //% block=end
    End = MB_SEEK_END
};

/**
* File system operations
*/
//% weight=5 color=#002050 icon="\uf0a0"
namespace files
{
// Initializes file system. Must be called before any FS operation.		
// built-in size computation for file system		
// does not take into account size changes		
// for compiled code		
void initFileSystem()		
{		
    if (MicroBitFileSystem::defaultFileSystem == NULL)		
    {		
        new MicroBitFileSystem(pxt::afterProgramPage());		
    }		
}    
/**
    * Appends text and a new line to a file
    * @param filename file name, eg: "output.txt"
    * @param text the string to append to the end of the file
    */
//% blockId="files_append_line" block="file %filename|append line %text"
//% blockExternalInputs=1 weight=90 blockGap=8
void appendLine(StringData *filename, StringData *text)
{
    initFileSystem();
    ManagedString fn(filename);
    MicroBitFile f(fn);
    if (text)
        f.append(text);
    f.append("\r\n");
    f.close();
}

/**
    * Appends text to a file
    * @param filename file name, eg: "output.txt"
    * @param text the string to append to the end of the file
    */
//% blockId="fs_append_string" block="file %filename|append string %text"
//% blockExternalInputs=1 weight=86 blockGap=8
void appendString(StringData *filename, StringData *text)
{
    if (!text)
        return;
    initFileSystem();
    ManagedString fn(filename);
    MicroBitFile f(fn);
    f.append(text);
    f.close();
}

/**
* Reads the content of the file to send it to serial
* @param filename file name, eg: "output.txt"
*/
//% blockId="fs_write_to_serial" block="file %filename|read to serial"
//% weight=80
void readToSerial(StringData* filename) {
    initFileSystem();
    ManagedString fn(filename);
    MicroBitFile f(fn);
    char buf[32];
    int read = 0;
    while((read = f.read(buf, 32 * sizeof(char))) > 0) {
         uBit.serial.send((uint8_t*)buf, read * sizeof(char));
    }   
    f.close();    
}

/**
    * Removes the file. There is no undo for this operation.
    * @param filename name of the file to remove, eg: "output.txt"
    */
//% blockId="fs_remove" block="file remove %filename"
//% weight=50 advanced=true
void remove(StringData *filename)
{
    initFileSystem();
    ManagedString fn(filename);
    MicroBitFileSystem::defaultFileSystem->remove(fn.toCharArray());
}

/**
* Creates a directory
* @param name full qualified path to the new directory
*/
//% advanced=true weight=10
//% blockId=files_create_directory block="files create directory %name"
void createDirectory(StringData* name) {
    initFileSystem();
    ManagedString fn(name);
    MicroBitFileSystem::defaultFileSystem->createDirectory(fn.toCharArray());
}

/** 
* Writes a number settings
* @param name name of the setting, must be filename compatible, e.g.: setting
* @param value value of the setting
*/
//% blockId=settings_write_number block="settings save number %name|as %value"
//% weight=20
void settingsSaveNumber(StringData* name, int value) {
    initFileSystem();
    MicroBitFile f(ManagedString("settings") + ManagedString(name), MB_WRITE | MB_CREAT);
    f.write(ManagedString(value));
    f.close();
}

/**
* Reads a number settings, -1 if not found.
* @param name name of the settings, must be filename compatible, e.g.: setting
*/
//% blockId=settings_read_number block="settings read number %name"
//% weight=19
int settingsReadNumber(StringData* name) {
    initFileSystem();
    MicroBitFile f(ManagedString("settings") + ManagedString(name), MB_READ);
    if (!f.isValid()) 
        return -1;
    ManagedString v;
    ManagedString buff;
    do {
        ManagedString buff = f.read(32);        
        v = v + buff;
    } while(buff.length() > 0);
    return atoi(v.toCharArray());
}

/**
*
*/
//% weight=0
int fsOpen(StringData* path) {
    initFileSystem();
    ManagedString fn(path);
    return MicroBitFileSystem::defaultFileSystem->open(fn.toCharArray(), MB_READ|MB_WRITE|MB_CREAT);
}

/**
*
*/
//% weight=0
int fsFlush(int fd) {
    if (fd <= 0) return fd;

    initFileSystem();
    return MicroBitFileSystem::defaultFileSystem->flush(fd);
}

/**
*
*/
//% weight=0
int fsClose(int fd) {
    if (fd <= 0) return fd;

    initFileSystem();
    return MicroBitFileSystem::defaultFileSystem->close(fd);
}

/**
*
*/
//% weight=0
int fsRemove(StringData* name) {
    initFileSystem();
    ManagedString fn(name);
    return MicroBitFileSystem::defaultFileSystem->remove(fn.toCharArray());
}

/**
*
*/
//% weight=0
int fsSeek(int fd, int offset, int flags) {
    if (fd <= 0) return fd;

    initFileSystem();
    return MicroBitFileSystem::defaultFileSystem->seek(fd, offset, flags);
}

/**
*
*/
//% weight=0
int fsWrite(int fd, Buffer buffer) {
    if (fd <= 0) return fd;

    initFileSystem();
    ManagedBuffer buf(buffer);
    auto pBuf = buf.leakData();
    return MicroBitFileSystem::defaultFileSystem->write(fd, pBuf->payload, pBuf->length);
}

}
