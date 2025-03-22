#include "Constants.h"

void DrawThickArrow(Vector2 start, Vector2 end, float thickness, Color color) {
    // Draw the main line
    DrawLineEx(start, end, thickness, color);

    // Direction vector
    Vector2 dir = { end.x - start.x, end.y - start.y };
    float length = sqrtf(dir.x * dir.x + dir.y * dir.y);
    if (length < 0.001f) return;
    // Normalize

    dir.x /= length;
    dir.y /= length;

    // Arrowhead settings
    float arrowSize = 14.0f;
    float arrowAngle = 30.0f * DEG2RAD; // 30 degrees in radians

    // End of arrow is the tip; we compute left/right edges
    Vector2 left, right;
    left.x = end.x - arrowSize * (dir.x * cosf(arrowAngle) - dir.y * sinf(arrowAngle));
    left.y = end.y - arrowSize * (dir.y * cosf(arrowAngle) + dir.x * sinf(arrowAngle));
    right.x = end.x - arrowSize * (dir.x * cosf(arrowAngle) + dir.y * sinf(arrowAngle));
    right.y = end.y - arrowSize * (dir.y * cosf(arrowAngle) - dir.x * sinf(arrowAngle));

    // Draw the filled triangle
    Vector2 points[3] = { end, left, right };
    DrawTriangle(points[0], points[1], points[2], color);
}

void DrawDropShadowRoundedRect(Rectangle rec, float roundness, int segments, float shadowOffset, Color shadowColor) {
    // We draw the same shape slightly offset to act like a shadow
    Rectangle shadowRec = rec;
    shadowRec.x += shadowOffset;
    shadowRec.y += shadowOffset;

    DrawRectangleRounded(shadowRec, roundness, segments, shadowColor);
}

Vector2 Lerp(Vector2 start, Vector2 end, float t) {
    Vector2 result;
    result.x = start.x + (end.x - start.x) * t;
    result.y = start.y + (end.y - start.y) * t;
    return result;
}

std::mt19937 rd(time(0));

int random(int l, int r) {
    if (l > r) return 0;
    return l + rd() % (r - l + 1);
}