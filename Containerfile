FROM fedora:latest

RUN sudo dnf install -y g++ gcc cmake libasan libubsan ncurses
