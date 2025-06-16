//extern "C" {
//    #include "sqcloud.h"  // Include the C library in C++ code
//}
#pragma once

#include <iostream>
#include <QApplication>
#include <QScrollArea>
#include <QGridLayout>
#include <QLabel>
#include <QWidget>
#include <QDir>
#include <QFileInfoList>
#include <QVBoxLayout>
#include <QMainWindow>
#include <QFileIconProvider>
#include <QIcon>
#include <QPixmap>
#include <QHBoxLayout>
#include <QSize>
#include <QProcess>
#include <QPushButton>
#include <QScreen>
#include <QKeyEvent>

#include <SFML/Graphics.hpp>

#include <cstdlib>

#include <filesystem>


void addMacOSBlurEffect(QWidget* widget, int width, int height);
