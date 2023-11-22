#pragma once
#ifndef COMPLEXPLANE_H
#define COMPLEXPLANE_H

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <cmath>


using namespace std;

enum class State { CALCULATING, DISPLAYING };

const unsigned int MAX_ITER = 128;
const float BASE_WIDTH = 4.0;
const float BASE_HEIGHT = 4.0;
const float BASE_ZOOM = 0.5;
const int W = sf::VideoMode::getDesktopMode().width / 2;
const int H = sf::VideoMode::getDesktopMode().height / 2;

class ComplexPlane : public sf::Drawable {
public:
    ComplexPlane(int pixelWidth, int pixelHeight);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void updateRender();
    void zoomIn();
    void zoomOut();
    void setCenter(sf::Vector2i mousePixel);
    void setMouseLocation(sf::Vector2i mousePixel);
    void loadText(sf::Text& text);

private:
    size_t countIterations(sf::Vector2f coord);
    void iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b);
    sf::Vector2f mapPixelToCoords(sf::Vector2i mousePixel);

    int m_pixelWidth;
    int m_pixelHeight;
    float m_aspectRatio;
    sf::Vector2f m_plane_center;
    sf::Vector2f m_plane_size;
    int m_zoomCount;
    State m_state;
    sf::VertexArray m_vArray;
    sf::Vector2f m_mouseLocation;
};

#endif
