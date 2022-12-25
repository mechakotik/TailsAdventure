#include "geometry.h"
#include "error.h"
#include "tools.h"

TA_Point::TA_Point(double newX, double newY)
{
    x = newX;
    y = newY;
}

TA_Point TA_Point::operator+(const TA_Point &rv)
{
    TA_Point res;
    res.x = x + rv.x;
    res.y = y + rv.y;
    return res;
}

TA_Point TA_Point::operator-(const TA_Point &rv)
{
    TA_Point res;
    res.x = x - rv.x;
    res.y = y - rv.y;
    return res;
}

TA_Point TA_Point::operator*(const TA_Point &rv)
{
    TA_Point res;
    res.x = x * rv.x;
    res.y = y * rv.y;
    return res;
}

double TA_Point::getDistance(TA_Point rv)
{
    return sqrt(pow(x - rv.x, 2) + pow(y - rv.y, 2));
}

double TA_Line::getLineEquation(TA_Point point)
{
    double res = (point.x - first.x) * (second.y - first.y) - (point.y - first.y) * (second.x - first.x);
    return res;
}

bool TA_Line::intersects(TA_Line rv)
{
    std::vector<double> sign(4);
    sign[0] = getLineEquation(rv.first);
    sign[1] = getLineEquation(rv.second);
    sign[2] = rv.getLineEquation(first);
    sign[3] = rv.getLineEquation(second);
    return sign[0] * sign[1] <= 0 && sign[2] * sign[3] <= 0;
}

void TA_Polygon::addVertex(TA_Point vertex)
{
    if(circle) {
        vertexList.clear();
        circle = false;
    }
    rect = false;
    vertexList.push_back(vertex);
    if(vertexList.size() == 4 && TA::equal(getVertex(1).x, getVertex(2).x) &&
       TA::equal(getVertex(1).y, getVertex(0).y) && TA::equal(getVertex(3).x, getVertex(0).x) &&
       TA::equal(getVertex(3).y, getVertex(2).y)) {
        rect = true;
    }
}

void TA_Polygon::setRectangle(TA_Point topLeft, TA_Point bottomRight)
{
    vertexList.clear();
    addVertex(topLeft);
    addVertex({bottomRight.x, topLeft.y});
    addVertex(bottomRight);
    addVertex({topLeft.x, bottomRight.y});
    rect = true;
}

void TA_Polygon::setCircle(TA_Point newVertex, double newRadius)
{
    radius = newRadius;
    circle = true;
    rect = false;
    vertexList.clear();
    vertexList.push_back(newVertex);
}

bool TA_Polygon::inside(TA_Point point)
{
    if(circle) {
        double distance = point.getDistance(vertexList[0] + position);
        return distance <= radius;
    }

    TA_Line ray;
    ray.first = point;
    ray.second = {1e5, point.y};
    int count = 0;

    for(int pos = 0; pos < int(vertexList.size()); pos ++) {
        TA_Line currentLine = {vertexList[pos] + position, vertexList[(pos + 1) % vertexList.size()] + position};
        if(ray.intersects(currentLine)) {
            count ++;
        }
    }

    return count % 2 == 1;
}

bool TA_Polygon::intersects(TA_Polygon rv)
{
    if(empty() || rv.empty()) {
        return false;
    }

    if(isRectangle() && rv.isRectangle()) {
        return getTopLeft().x < rv.getBottomRight().x && getBottomRight().x > rv.getTopLeft().x &&
               getTopLeft().y < rv.getBottomRight().y && getBottomRight().y > rv.getTopLeft().y;
    }

    for(TA_Point vertex : vertexList) {
        if(rv.inside(vertex + position)) {
            return true;
        }
    }
    for(TA_Point vertex : rv.vertexList) {
        if(inside(vertex + rv.position)) {
            return true;
        }
    }
    return false;
}

double TA_Polygon::getDistance(TA_Polygon rv)
{
    double distance = 1e9;
    for(TA_Point a : vertexList) {
        for(TA_Point b : rv.vertexList) {
            distance = std::min(distance, a.getDistance(b));
        }
    }
    return distance;
}

TA_Point TA_Polygon::getVertex(int pos)
{
    if(pos >= vertexList.size()) {
        handleError("Vertex index %i is out of range [0; %i)", pos, vertexList.size());
    }
    return vertexList[pos] + position;
}
