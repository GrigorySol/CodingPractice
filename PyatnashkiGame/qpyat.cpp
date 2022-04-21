#include "qpyat.h"
#include "ui_qpyat.h"
#include "vector"
#include "algorithm"
#include "random"

QPyat::QPyat(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::QPyat)
{
    ui->setupUi(this);
    this->setFixedSize(QSize(440,400));

    createPuzzle();
}

QPyat::~QPyat()
{
    delete ui;
}

void QPyat::createPuzzle()
{
    QGridLayout *gameTable = new QGridLayout(this); // Create Layout for tiles
    gameTable->setSpacing(0);

    const QSize tileSize = QSize(100, 100);         // Set fixed size for tiles
    QFont tileFont = this->font();
    tileFont.setPointSize(24);                      // Set bigger font size

    std::vector<int> tileNames{};                   // Create shuffled vector with tile numbers
    for (int x{1}; x<17; x++) tileNames.push_back(x);
    auto rd = std::random_device {};
    auto rng = std::default_random_engine {rd()};
    std::shuffle(std::begin(tileNames), std::end(tileNames), rng);

    int vectorIndex {0};
    for (int i{0}; i<4; i++) {
        for (int k{0}; k<4; k++) {
            if(tileNames[vectorIndex] == 16) {      // Skip tile with number 16
                vectorIndex++;
                continue;
            }
            QPushButton *tile = new QPushButton(this);                         // Create tile
            tile->setFixedSize(tileSize);                                      // Fix tile size
            tile->setText(QString::number(tileNames[vectorIndex]));            // Set a name
            tile->setFont(tileFont);

            gameTable->addWidget(tile, 0+i, k);     // Add tile to the game table
            vectorIndex++;
        }
    }
}
