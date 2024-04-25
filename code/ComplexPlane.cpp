#include "ComplexPlane.h"
#include <cmath>
#include <sstream>
#include <complex>


ComplexPlane::ComplexPlane(int w, int h) {
    m_pixel_size = {w, h};
    m_aspectRatio = (float)w / h;
    m_plane_size = {BASE_WIDTH, BASE_HEIGHT * m_aspectRatio};
    m_plane_center = {0,0};
    m_zoomCount = 0;
    m_state = State::CALCULATING;
    m_vArray.setPrimitiveType(Points);
    m_vArray.resize(w * h);
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const {
    target.draw(m_vArray);
}

void ComplexPlane::zoomIn() {
    m_zoomCount++;
    float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = {x, y};
    m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut() {
    m_zoomCount--;
    float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = {x, y};
    m_state = State::CALCULATING;
}

void ComplexPlane::setCenter(Vector2f mousePixel) {
    m_plane_center = mapPixelToCoords(mousePixel);
    m_state = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2f mousePixel) {
    m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(Text& text) {
    complex<double> center(m_plane_center.x, m_plane_center.y);
    complex<double> cursore(m_mouseLocation.x, m_mouseLocation.y);
    stringstream str;
    str << "Mandelbrot Set\nCenter: " << center << "\nCursore: " << cursore << "\nLeft-click to Zoom in\nRight-click to Zoom out\n";
    text.setString(str.str());
    text.setCharacterSize(24);
    text.setFillColor(Color::White);
    text.setPosition(10, 20);
}

void ComplexPlane::updateRender() {
    if (m_state == State::CALCULATING) {
        int width = m_pixel_size.x;
        int height = m_pixel_size.y;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                m_vArray[j + i * width].position = {(float)j, (float)i};
                Vector2f coords = mapPixelToCoords({(float)j, (float)i});
                int numOfIteration = coutIterations(coords);
                Uint8 r, g, b;
		        r = numOfIteration % 256;
		        g = numOfIteration % 256;
		        b = numOfIteration % 256;
                m_vArray[j + i * width].color = {r, g, b};
            }
        }
        m_state = State::DISPLAYING;
    }
}

size_t ComplexPlane::coutIterations(Vector2f coord) {
     complex<float> c(coord.x, coord.y);
    complex<float> z(0, 0);
    size_t iterations = 0;

    while (abs(z) <= 2.0 && iterations < MAX_ITER) {
        z = z * z + c;
        ++iterations;
    }

    return iterations;
}

void ComplexPlane::iterationToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b) {
    const size_t MAX_ITER = 255;
    if (count >= MAX_ITER) {
        r = g = b = 0; // Black color
    } else {
        size_t region = count / (MAX_ITER / 5); // Divide into 5 regions
        switch (region) {
            case 0: // Purple / Blue
                r = 0; g = 0; b = count;
                break;
            case 1: // Turquoise
                r = 0; g = count; b = count;
                break;
            case 2: // Green
                r = 0; g = count; b = 0;
                break;
            case 3: // Yellow
                r = count; g = count; b = 0;
                break;
            case 4: // Red
                r = count; g = 0; b = 0;
                break;
        }
    }
}


Vector2f ComplexPlane::mapPixelToCoords(Vector2f mousePixel) {
    Vector2f coords;
    coords.x = ((mousePixel.x - 0.f) / (m_pixel_size.x - 0.f)) * (m_plane_size.x - -m_plane_size.x) + (m_plane_center.x - m_plane_size.x / 2.0);
    coords.y = ((mousePixel.y - m_pixel_size.y) / (0.f - m_pixel_size.y)) * (m_plane_size.y - -m_plane_size.y) + (m_plane_center.y - m_plane_size.y / 2.0);
    return coords;
}
