/*
RUNNING on VSCODE

Press Cmd + Shift + P to open Command Palette.
Type CMake: Configure and select it.
If prompted, choose Clang as the compiler.
Open Command Palette again and select CMake: Build.
Finally, select CMake: Run Without Debugging.

OR

Run with one click on the bottom button

*/

#include "header.h"

namespace fs = std::filesystem;

class EscapeFilter : public QObject {
    protected:
        bool eventFilter(QObject* obj, QEvent* event) override {
            if (event->type() == QEvent::KeyPress) {
                // This is safe now because we've checked the type
                QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
                if (keyEvent->key() == Qt::Key_Escape) {
                    qApp->quit();
                    return true;
                }
            }
            return QObject::eventFilter(obj, event);
        }
    };
    
QWidget* createAppGrid(const QString& path, int columns = 7) {
    QWidget* container = new QWidget;
    QGridLayout* grid = new QGridLayout(container);
    grid->setSpacing(20);
    grid->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    grid->setColumnStretch(columns, 1);
    container->setAttribute(Qt::WA_TranslucentBackground);
    container->setStyleSheet("background: transparent;");

    QDir dir(path);
    QFileInfoList entries = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);

    QFileIconProvider iconProvider;
    int row = 0, col = 0;

    for (const QFileInfo& entry : entries) {
        if (!entry.fileName().endsWith(".app"))
            continue;

        
        
        // Create a button instead of a label to make it clickable
        QPushButton* appButton = new QPushButton;
        appButton->setFlat(true);
        appButton->setCursor(Qt::PointingHandCursor);
        appButton->setStyleSheet("color: white; text-align: center; padding: 8px;");

        // Vertical layout inside button
        QVBoxLayout* appLayout = new QVBoxLayout(appButton);
        appLayout->setSpacing(5);
        appLayout->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

        // App icon
        QIcon icon = iconProvider.icon(entry);
        QLabel* iconLabel = new QLabel;
        QPixmap pixmap = icon.pixmap(64, 64);
        iconLabel->setPixmap(pixmap);
        iconLabel->setAlignment(Qt::AlignCenter);
        iconLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);


        // App name processing (remove .app, trim to 15 chars)
        QString fullName = entry.fileName();
        if (fullName.endsWith(".app", Qt::CaseInsensitive))
            fullName.chop(4);
        if (fullName.length() > 15)
            fullName = fullName.left(12) + "...";

        QLabel* nameLabel = new QLabel(fullName);
        nameLabel->setAlignment(Qt::AlignCenter);
        nameLabel->setStyleSheet("color: white;");
        nameLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);

        appButton->setMinimumSize(145, 145);

        appLayout->addWidget(iconLabel);
        appLayout->addWidget(nameLabel);
        appButton->setLayout(appLayout);

        // Connect button click to open the app using QProcess and 'open' command
        QString appPath = entry.absoluteFilePath();
        QObject::connect(appButton, &QPushButton::clicked, [appPath]() {
            QProcess::startDetached("open", QStringList() << appPath); // Launch the app
            qApp->quit(); // Quit launchpad
        });

        grid->addWidget(appButton, row, col);

        if (++col >= columns) {
            col = 0;
            ++row;
            
        }
        //grid->addWidget(appButton, row, col, Qt::AlignCenter);
        
    }
    
    // Let columns stretch evenly
    //for (int c = 0; c < columns; ++c) {
    //    grid->setColumnStretch(c, 1);
    //}
    
    container->setLayout(grid);
    return container;
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QMainWindow window;
    window.setWindowTitle("Launchpad by Jarosoft");

    // Scroll area
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("background: transparent;");
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setStyleSheet("QScrollBar:vertical { width: 0px; background: transparent; }");
    scrollArea->setAttribute(Qt::WA_TranslucentBackground);
    //scrollArea->viewport()->setAttribute(Qt::WA_TranslucentBackground);
    scrollArea->setStyleSheet("");

    // Grid inside scroll area
    QWidget* appGrid = createAppGrid("/Applications");
    scrollArea->setWidget(appGrid);

    // Central widget
    QWidget* central = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(central);
    layout->addWidget(scrollArea);
    layout->setContentsMargins(10, 10, 10, 10);
    central->setLayout(layout);
    central->setAttribute(Qt::WA_TranslucentBackground);
    central->setStyleSheet("background: transparent;");
    
    // Create transparent background widget
    QWidget* blurBackground = new QWidget(&window);
    blurBackground->setGeometry(0, 0, 1200, 750);
    blurBackground->setAttribute(Qt::WA_TranslucentBackground);
    blurBackground->setStyleSheet("background: transparent;");
    blurBackground->lower();  // Make sure it's behind all other widgets
    blurBackground->show();
    //blurBackground->setStyleSheet("background-color: rgba(255, 0, 0, 50);");

    // Apply the blur to the background
    addMacOSBlurEffect(blurBackground, 1200, 750);

    window.setCentralWidget(central);
    window.setFixedSize(1200, 750);
    window.setAttribute(Qt::WA_TranslucentBackground);
    window.setWindowFlag(Qt::FramelessWindowHint);

    // Center the window
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int x = (screenGeometry.width() - window.width()) / 2;
    int y = (screenGeometry.height() - window.height()) / 2;
    window.move(x, y);


    EscapeFilter* filter = new EscapeFilter();
    window.installEventFilter(filter);

    
    //addMacOSBlurEffect(&window, 1200, 750);
    window.show();

    return app.exec();
}