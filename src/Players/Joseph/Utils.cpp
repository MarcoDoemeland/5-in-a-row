#include <Players/Joseph/Utils.h>

#include <fstream>
#include <math.h>
#include <iostream>

namespace FIAR
{

namespace joseph
{

ColorRGB ColorRGB::fromHSV(const ColorHSV &in)
{
    // Constants
    const double maxValue = 255.;
    const double regionSize = 60.;

    // Input
    const double h = in.hue;
    const double s = in.saturation / maxValue;
    const double v = in.value / maxValue;

    // Calculations
    const double region = h / regionSize;
    const double c = v * s;
    auto modulus = [](const double lhs, const double rhs)
    { return lhs - (std::floor(lhs / rhs)) * rhs; };
    const double x = c * (1 - std::abs(modulus(region, 2.) - 1));
    const double r1 = region < 1. || region >= 5. ? c
                    : region >= 2. && region < 4. ? 0. : x;
    const double g1 = region >= 1. && region < 3. ? c
                    : region >= 4. ? 0. : x;
    const double b1 = region >= 3. && region < 5. ? c
                    : region < 2. ? 0. : x;
    const double m = v - c;
    auto fractionToValue = [maxValue](const double fraction)
    { return static_cast<uint8_t>(maxValue * fraction); };

    // Output
    return ColorRGB{ fractionToValue(r1 + m), fractionToValue(g1 + m), fractionToValue(b1 + m)};
}

// Constructor
Image::Image(size_t w, size_t h)
    : m_w{ w }
    , m_h{ h }{
    if(m_w && m_h){
        // Defining the count of bytes in a row
        m_rowBytes = m_w * 3;
        while(m_rowBytes % 4 != 0) ++m_rowBytes;
        // Defining the count of data bytes
        m_dataBytes   = m_h * m_rowBytes;
        // Defining the count of bytes in the file
        m_fileBytes = s_gHeaderBytes + s_iHeaderBytes + m_dataBytes;

        // Building the image
        m_image       = new int8_t[m_fileBytes];
        // Defining the general header
        m_magicNumber = (int16_t*) m_image;
        m_fileSize    = (int32_t*) (m_magicNumber + 1);
        m_id1         = (int16_t*) (m_fileSize    + 1);
        m_id2         = (int16_t*) (m_id1         + 1);
        m_imageOffset = (int32_t*) (m_id2         + 1);
        // Defining the image header
        m_iHeaderSize = (int32_t*) (m_imageOffset + 1);
        m_imageW      = (int32_t*) (m_iHeaderSize + 1);
        m_imageH      = (int32_t*) (m_imageW      + 1);
        m_colorPlanes = (int16_t*) (m_imageH      + 1);
        m_bitsPerPx   = (int16_t*) (m_colorPlanes + 1);
        m_comprMethod = (int32_t*) (m_bitsPerPx   + 1);
        m_imageSize   = (int32_t*) (m_comprMethod + 1);
        m_horiReso    = (int32_t*) (m_imageSize   + 1);
        m_vertReso    = (int32_t*) (m_horiReso    + 1);
        m_colorCnt    = (int32_t*) (m_vertReso    + 1);
        m_impColorCnt = (int32_t*) (m_colorCnt    + 1);
        // Data
        m_data        = (uint8_t*) (m_impColorCnt + 1);

        // Setting the magic number
        *m_magicNumber = 0x4d42;
        // Setting the size
        *m_fileSize    = m_fileBytes;
        // Setting the IDs
        *m_id1         = 0;
        *m_id2         = 0;
        // Setting the image offset
        *m_imageOffset = s_gHeaderBytes + s_iHeaderBytes;
        // Setting the size of the image header
        *m_iHeaderSize = s_iHeaderBytes;
        // Setting the dimensions of the pic
        *m_imageW      = m_w;
        *m_imageH      = m_h;
        // Setting the number of color planes
        *m_colorPlanes = 1;
        // Setting the number of bits per pixel
        *m_bitsPerPx   = 24;
        // Setting the compression method (0 is none)
        *m_comprMethod = 0;
        // Setting the image size
        *m_imageSize   = m_dataBytes;
        // Setting the resolution (px per meter) --> 2cm per pixel
        *m_horiReso    = 50;
        *m_vertReso    = 50;
        // Setting the number of colors in the color palette: 0 to default to 2n
        *m_colorCnt    = 0;
        // Setting the number of important colors used: 0 when every color is important
        *m_impColorCnt = 0;

        // Resetting the image
        reset();
    }
}
// Destructor
Image::~Image(){
    delete[] m_image;
}

// Resetting the image
void Image::reset(uint8_t r, uint8_t g, uint8_t b){
    // Setting all data to 0
    if(m_image){
        for(size_t y{ 0u }; y < m_h; ++y){
            m_pxRW = m_data + (y * m_rowBytes);
            for(size_t x{ 0u }; x < m_w; ++x){
                *(m_pxRW + 2) = r;
                *(m_pxRW + 1) = g;
                *(m_pxRW + 0) = b;
                m_pxRW += 3;
            }
            while(m_pxRW - m_data < m_rowBytes) *(m_pxRW++) = 0;
        }
    }
}
void Image::reset(const ColorRGB& color){
    reset(color.red, color.green, color.blue);
}
void Image::reset(){
    reset(0u, 0u, 0u);
}
// Setting a pixel value
void Image::setPixel(size_t x, size_t y, uint8_t r, uint8_t g, uint8_t b){
    // Invalid coordinates
    if(x > m_w || y > m_h) return;
    // Getting the address
    m_pxRW = m_data + (y * m_rowBytes + x * 3);
    // Writing the pixel
    *(m_pxRW + 2) = r;
    *(m_pxRW + 1) = g;
    *(m_pxRW + 0) = b;
}
void Image::setPixel(size_t x, size_t y, const ColorRGB& color){
    setPixel(x, y, color.red, color.green, color.blue);
}
// Returning a pixel value
ColorRGB Image::getPixel(size_t x, size_t y){
    // Invalid coordinates
    if(x > m_w || y > m_h) return { 0u, 0u, 0u };
    // Getting the address
    m_pxRW = m_data + (y * m_rowBytes + x * 3);
    // Returning the result
    return { *(m_pxRW + 2), *(m_pxRW + 1), *(m_pxRW + 0) };
}
// Saving the image
void Image::save(const std::string& name){
    if(name.size() <= 0)
        return;
    std::ofstream stream(name + ".bmp", std::ofstream::binary);
    stream << *this;
    //stream.flush();
    stream.close();
}
// << operator overload
std::ostream& operator<<(std::ostream& stream, const Image& image){
    if(image.m_image){
        for(int i{ 0 }; i < image.m_fileBytes; ++i)
            stream << image.m_image[i];
    }
    return stream;
}

TimeEvaluation::TimeEvaluation()
    : start{ std::chrono::system_clock::now() }
{
}
std::chrono::microseconds TimeEvaluation::Duration() const
{
    return std::chrono::duration_cast<std::chrono::microseconds>(Now() - start);
}
int64_t TimeEvaluation::ElapsedUs() const
{
    return Duration().count();
}
std::chrono::time_point<std::chrono::system_clock> TimeEvaluation::Now() const
{
    return std::chrono::system_clock::now();
}
std::ostream& operator<<(std::ostream& stream, const TimeEvaluation& eval)
{
    stream << eval.ElapsedUs() << " us";
    return stream;
}

} // End namespace joseph

} // End namespace FIAR
