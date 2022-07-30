#include<iostream>
#include <algorithm>
#include <vector>

using namespace std;

struct point {
    long int x;
    long int y;
};

void swap(point &p1, point &p2) {
    point temp = p1;
    p1 = p2;
    p2 = temp;
}

long long int ccw(const point &p1, const point &p2, const point &p3) {
    return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}

long long int dist(const point p1, const point p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

struct CompareAngle {
    point p0;

    bool operator()(point &p1, point &p2) const {
        long long int ccw_result = ccw(p0, p1, p2);
        if (ccw_result == 0) return dist(p0, p1) < dist(p0, p2);
        return ccw_result > 0;
    }

};

int remove_collinear(vector<point> &v, point p0) {
    int i = 1;
    for (int j = 1; j < static_cast<int>(v.size()); j++) {
        while (j < static_cast<int>(v.size() - 1) && ccw(p0, v[j], v[j + 1]) == 0) j++;
        v[i] = v[j];
        i++;
    }
    return i;
}

int main() {
    int number;
    cin >> number;
    if (number == 0) return 0;
    point input;
    vector<point> x;
    vector<point> s;
    while (number > 0) {
        cin >> input.x >> input.y;
        x.push_back(input);
        number--;
    }

    if (number == 1) cout << x[0].x << x[0].y;
    int min_index = 0;
    long int min_y = x[0].y;

    for (auto ptr = x.begin() + 1; ptr < x.end(); ptr++) {
        if (ptr->y < min_y || (ptr->y == min_y && ptr->x < x[0].x)) {
            min_y = ptr->y;
            min_index = static_cast<int>(ptr - x.begin());
        }
    }

    point p_0 = x[min_index];

    CompareAngle comp = {p_0};
    swap(x[0], x[min_index]);
    sort(x.begin() + 1, x.end(), comp);
    int size = remove_collinear(x, p_0);

    // if the case that the size is two containing two duplicated points
    if (p_0.x == x[1].x && p_0.y == x[1].y) size = 1;

    for (int i = 0; i < size; i++) {
        while (s.size() > 1 && ccw(*(s.end() - 2), *(s.end() - 1), x[i]) <= 0) {
            s.pop_back();
        }
        s.push_back(x[i]);
    }
    for (auto points: s) {
        cout << points.x << " " << points.y << endl;
    }
    return 0;
}

