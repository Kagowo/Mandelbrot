#include "ComplexPlane.h"

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
    m_pixelWidth = (pixelWidth);
      m_pixelHeight = (pixelHeight);
      m_aspectRatio = static_cast<float>(pixelWidth) / static_cast<float>(pixelHeight);
      m_plane_center = {0, 0};
      m_zoomCount = 0;
      m_state = State::CALCULATING);
    m_plane_size = {BASE_WIDTH, BASE_HEIGHT * m_aspectRatio};
    m_vArray.setPrimitiveType(sf::Points);
    m_vArray.resize(pixelWidth * pixelHeight);
    updateRender();
}

void ComplexPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_vArray, states);
}

void ComplexPlane::updateRender() {
    if (m_state == State::CALCULATING) {
        for (int i = 0; i < m_pixelHeight; i++) {
            for (int j = 0; j < m_pixelWidth; j++) {
                m_vArray[j + i * m_pixelWidth].position = {static_cast<float>(j), static_cast<float>(i)};
                sf::Vector2f coords = mapPixelToCoords({j, i});
                size_t iterations = countIterations(coords);
                sf::Uint8 r, g, b;
                iterationsToRGB(iterations, r, g, b);
                m_vArray[j + i * m_pixelWidth].color = {r, g, b};
            }
        }
        m_state = State::DISPLAYING;
    }
}

void ComplexPlane::zoomIn() {
    ++m_zoomCount;
    float xSize = BASE_WIDTH * std::pow(BASE_ZOOM, m_zoomCount);
    float ySize = BASE_HEIGHT * m_aspectRatio * std::pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = {xSize, ySize};
    m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut() {
    if (m_zoomCount > 0) {
        m_zoomCount--;
        float xSize = BASE_WIDTH * std::pow(BASE_ZOOM, m_zoomCount);
        float ySize = BASE_HEIGHT * m_aspectRatio * std::pow(BASE_ZOOM, m_zoomCount);
        m_plane_size = {xSize, ySize};
        m_state = State::CALCULATING;
    }
}

void ComplexPlane::setCenter(sf::Vector2i mousePixel) {
    sf::Vector2f coords = mapPixelToCoords(mousePixel);
    m_plane_center = coords;
    m_state = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(sf::Vector2i mousePixel) {
    m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(sf::Text& text) {
    stringstream mandelInfo;
    mandelInfo << "Mandelbrot Set\n";
    mandelInfo << "Center: (" << m_plane_center.x << ", " << m_plane_center.y << ")\n";
    mandelInfo << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")\n";
    mandelInfo << "Left-click to Zoom in\n";
    mandelInfo << "Right-click to Zoom out\n";
    text.setString(mandelInfo.str());
}

size_t ComplexPlane::countIterations(sf::Vector2f coord) {
    sf::Vector2f z = coord;
    size_t iterations = 0;

    while (z.x * z.x + z.y * z.y < 4.0 && iterations < MAX_ITER) {
        sf::Vector2f newZ;
        newZ.x = z.x * z.x - z.y * z.y + coord.x;
        newZ.y = 2.0 * z.x * z.y + coord.y;
        z = newZ;
        iterations++;
    }

    return iterations;
}

void ComplexPlane::iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) {
 if (count == MAX_ITER) 
    {
        r = g = b = 0;
    } 
    else 
    {
        const int regionSize = MAX_ITER / 5;
        int region = count / regionSize;

        switch(region)
        {
            case 0:
                r = 128 - (count % regionSize);
                g = 0;
                b = 255;
                break;
            case 1:
                r = 0;
                g = 255;
                b = 255 - (count % regionSize);
                break;
            case 2:
                r = 0;
                g = 255;
                b = 0;
                break;
            case 3:
                r = 255;
                g = 255 - (count % regionSize);
                b = 0;
                break;
            case 4:
                r = 255;
                g = 0;
                b = 0;
                break;
            default:
                r = g = b = 255;
                break;
        }
    }
}

sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i mousePixel) {
    float x = ((mousePixel.x - 0) / static_cast<float>(m_pixelWidth - 0)) * m_plane_size.x +
              (m_plane_center.x - m_plane_size.x / 2.0);
    float y = ((mousePixel.y - m_pixelHeight) / static_cast<float>(0 - m_pixelHeight)) * m_plane_size.y +
              (m_plane_center.y - m_plane_size.y / 2.0);
    return {x, y};
}
