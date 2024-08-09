#include <iostream>
#include <cmath>

class Point {
    public:
        Point() {x = 0.0;};
        Point(double x): x(x) {};
        virtual void print()=0;
        virtual double getX() const {return x;};
    private:
        double x = 0.0;
};

class Point2d: public Point {
    public:
        Point2d() {x = 0.0; y = 0.0;};
        explicit Point2d(double x, double y): x(x), y(y) {};
        void print() override { std::cout << "X: " << x << " Y: " << y << std::endl; };
        double getX() const override {return x;};
        double getY() const {return y;};

        // second part
        double distance_to(const Point2d& p) {
            return (double)std::sqrt(pow((this->getX() - p.getX()), 2.0) +  pow((this->getY() - p.getY()), 2.0));
        }
    private:
        double x = 0.0;
        double y = 0.0;
};

int main() {
    Point2d first{};
    Point2d second{ 3.0, 4.0 };

    first.print();
    second.print();

    std::cout << "Distance to the point 2: " << first.distance_to(second) << std::endl;

    return 0;
}
