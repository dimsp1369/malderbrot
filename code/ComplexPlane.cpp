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
        for (int j = 0; j < height; j++) {
            for (int i = 0; i < width; i++) {
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

int ComplexPlane::coutIterations(Vector2f coord) {
    return 1;
}
void ComplexPlane::iterationToRGB(size_t cout, Uint8& r, Uint8& g, Uint8& b) {}

Vector2f ComplexPlane::mapPixelToCoords(Vector2f mousePixel) {
    Vector2f v(mousePixel.x, mousePixel.y);
    return v;
}