// -----------------------------------------------------------------------------
// 5iar -- Utils.h
// -----------------------------------------------------------------------------
//
// ...
//

#ifndef __FIAR_PLAYERS_JOSEPH_UTILS_H__
#define __FIAR_PLAYERS_JOSEPH_UTILS_H__

#include <chrono>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <string>

namespace FIAR
{

namespace joseph
{

// Directions
enum JDirections{
    dir_x,// Along x
    dir_d,// Diagonal
    dir_y,// Along y
    dir_a,// Antidiagonal
    dir_count,
    dir_nodir// Placed after count, since it is not a direction
};

struct ColorHSV
{
    uint16_t hue      { 0u };
    uint8_t saturation{ 0u };
    uint8_t value     { 0u };
};

struct ColorRGB
{
    uint8_t red  { 0u };
    uint8_t green{ 0u };
    uint8_t blue { 0u };

    static ColorRGB fromHSV(const ColorHSV& in);
};

class Image
{
public:
    // Constructors
    Image(size_t w, size_t h);
    Image(const Image& ref) = delete;
    Image() = delete;

    // Destructor
    ~Image();

    // Resetting the image
    void reset(uint8_t r, uint8_t g, uint8_t b);
    void reset(const ColorRGB& color);
    void reset();
    // Setting a pixel value
    void setPixel(size_t x, size_t y, uint8_t r, uint8_t g, uint8_t b);
    void setPixel(size_t x, size_t y, const ColorRGB& color);
    // Returning a pixel value
    ColorRGB getPixel(size_t x, size_t y);
    // Saving the image
    void save(const std::string& name);
    // << operator overload
    friend std::ostream& operator<<(std::ostream& stream, const Image& image);

private:
    // Dimensions
    size_t m_w{ 0 };
    size_t m_h{ 0 };
    // Size of the header
    static const int32_t s_gHeaderBytes{ 14 };
    // Size of the image header
    static const int32_t s_iHeaderBytes{ 40 };
    // Size of a row
    int32_t m_rowBytes{ 0 };
    // Size of the data
    int32_t m_dataBytes{ 0 };
    // Size of the file
    int32_t m_fileBytes{ 0 };

    // Actual image
    int8_t* m_image{ nullptr };

    // General header:
    // 0x0000	2 octets	le nombre magique correspondant à l'utilisation du fichier BMP
    int16_t* m_magicNumber{ nullptr };
    // BM - Windows 3.1x, 95, NT, etc.
    // BA - OS/2 Bitmap Array
    // CI - OS/2 Icône Couleur (Color Icon)
    // CP - OS/2 Pointeur Couleur (Color Pointer)
    // IC - OS/2 Icône (Icon)
    // PT - OS/2 Pointeur (Pointer)
    // 0x0002	4 octets	la taille du fichier BMP en octets
    int32_t* m_fileSize{ nullptr };
    // 0x0006	2 octets	réservé pour l'identifiant de l'application qui a créé le fichier
    int16_t* m_id1{ nullptr };
    // 0x0008	2 octets	réservé pour l'identifiant de l'application qui a créé le fichier
    int16_t* m_id2{ nullptr };
    // 0x000A	4 octets	l'offset (l'adresse de départ) du contenu du BMP
    int32_t* m_imageOffset{ nullptr };

    // Image header:
    int32_t* m_iHeaderSize{ nullptr };
    int32_t* m_imageW{ nullptr };
    int32_t* m_imageH{ nullptr };
    int16_t* m_colorPlanes{ nullptr };
    int16_t* m_bitsPerPx{ nullptr };
    int32_t* m_comprMethod{ nullptr };
    int32_t* m_imageSize{ nullptr };
    int32_t* m_horiReso{ nullptr };
    int32_t* m_vertReso{ nullptr };
    int32_t* m_colorCnt{ nullptr };
    int32_t* m_impColorCnt{ nullptr };

    // Data
    uint8_t* m_data{ nullptr };

    // Pointer for reading / writing pixels
    uint8_t* m_pxRW{ nullptr };
};

struct TimeEvaluation
{
    TimeEvaluation();

    std::chrono::microseconds Duration() const;
    int64_t ElapsedUs() const;

private:
    std::chrono::time_point<std::chrono::system_clock> Now() const;
    std::chrono::time_point<std::chrono::system_clock> start;
};

std::ostream& operator<<(std::ostream& stream, const TimeEvaluation& eval);

class Logger
{
public:
    // Constructor: the Logger will take the filename only if logging is active
    Logger(const std::string& filename, bool activate = false)
        : m_logEnabled{ activate }
    {
        if (m_logEnabled)
            m_fileStream = std::ofstream(filename);
    }

    // Logging stuff
    inline void log(const std::string& str){
        if(!m_logEnabled)
            return;
        m_fileStream << str << '\n';
        std::cout << str << std::endl;
    }

    // Logging text + template
    template<typename T>
    inline void log(const std::string& str, const T& var){
        if(!m_logEnabled)
            return;
        m_fileStream << str << ' ' << var << '\n';
        std::cout << str << ' ' << var << std::endl;
    }

private:
    // Logging variables
    bool m_logEnabled;
    std::ofstream m_fileStream;
};

} // End namespace joseph

} // End namespace FIAR

#endif // __FIAR_PLAYERS_JOSEPH_UTILS_H__