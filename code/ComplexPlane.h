#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

const unsigned int MAX_ITER = 64;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;
enum State { CALCULATING, DISPLAYING };

class ComplexPlane : public Drawable {
    public:
        ComplexPlane(int w, int h);
        void draw(RenderTarget& target, RenderStates states) const override;
        void zoomIn();
        void zoomOut();
        void setCenter(Vector2f mousePixel);
        void setMouseLocation(Vector2f mousePixel);
        void loadText(Text& text);
        void updateRender();

    private:
        VertexArray m_vArray;
        State m_state;
        Vector2f m_mouseLocation;
        Vector2i m_pixel_size;
        Vector2f m_plane_center;
        Vector2f m_plane_size;
        int m_zoomCount;
        float m_aspectRatio;

        size_t coutIterations(Vector2f coord);
        void iterationToRGB(size_t cout, Uint8& r, Uint8& g, Uint8& b);
        Vector2f mapPixelToCoords(Vector2f mousePixel);
};