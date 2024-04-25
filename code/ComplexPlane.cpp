#include "ComplexPlane.h"
#include <cmath>
#include <sstream>
#include <complex>


ComplexPlane::ComplexPlane(int w, int h) {
    m_pixel_size = {w, h};
    m_aspectRatio = (float)w / (float)h;
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
    ++m_zoomCount;
    float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = Vector2f(x, y);
    m_state = State::CALCULATING;
}

void ComplexPlane::zoomOut() {
    --m_zoomCount;
    float x = BASE_WIDTH * pow(BASE_ZOOM, m_zoomCount);
    float y = BASE_HEIGHT * m_aspectRatio * pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = Vector2f(x, y);
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
                m_vArray[j + i * width].position = Vector2f((float)j, (float)i);
                Vector2f coords = mapPixelToCoords(Vector2f((float)j, (float)i));
                size_t numOfIteration = coutIterations(coords);
                Uint8 r, g, b;
                iterationToRGB(numOfIteration, r, g, b);
                m_vArray[j + i * width].color = Color(r, g, b);
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

    // If the point is within the Mandelbrot set, return 0 iterations
    return (abs(z) <= 2.0) ? 0 : iterations;
}


void ComplexPlane::iterationToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b) {
    if (count >= MAX_ITER) {
        r = g = b = 0;
    } else {
        if (count < MAX_ITER / 4) {
            // Purple
            r = 128 * (count % 4);
            g = 0;
            b = 255;
        } else if (count < 2 * MAX_ITER / 4) {
            // Turquoise
            r = 0;
            g = 128 * (count % 4);
            b = 255;
        } else if (count < 3 * MAX_ITER / 4) {
            // Green
            r = 0;
            g = 255;
            b = 128 * (count % 4);
        } else {
            // Yellow
            r = 128 * (count % 4);
            g = 255;
            b = 128 * (count % 4);
        }
    }
}



Vector2f ComplexPlane::mapPixelToCoords(Vector2f mousePixel) {
    float x_min = m_plane_center.x - m_plane_size.x / 2.0;
    float x_max = m_plane_center.x + m_plane_size.x / 2.0;
    float y_min = m_plane_center.y - m_plane_size.y / 2.0;
    float y_max = m_plane_center.y + m_plane_size.y / 2.0;

    // Map the pixel location to a coordinate in the complex plane
    float x = ((mousePixel.x - 0.f) / static_cast<float>(m_pixel_size.x)) * (x_max - x_min) + x_min;
    float y = ((m_pixel_size.y - mousePixel.y) / static_cast<float>(m_pixel_size.y)) * (y_max - y_min) + y_min;

    return Vector2f(x, y);
}
