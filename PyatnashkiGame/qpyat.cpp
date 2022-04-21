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
    init();
}

QPyat::~QPyat()
{
    delete ui;
}

void QPyat::init()
{
    this->setFixedSize(QSize(460,500));
    textFont = this->font();
    textFont.setPointSize(24);                      // Set bigger font size

    gameTable = new QFrame(this);
    gameTable->setFixedSize(QSize(460,420));        // On my macbook I have to use this magic numbers to make tiles look square
    gameLayout = new QGridLayout(gameTable);        // Create Layout for tiles
    gameLayout->setSpacing(0);

    createPuzzle();

    btnNewGame = new QPushButton(this);
    btnNewGame->setText("New Game");
    btnNewGame->setFont(textFont);
    ui->qPyatLayout->addWidget(btnNewGame, 0, Qt::AlignBottom);
    connect(btnNewGame, &QPushButton::clicked, this, &QPyat::startNewGame);
}

void QPyat::createPuzzle()
{
    const QSize tileSize = QSize(100, 100);         // Set fixed size for tiles

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
            QPushButton *tile = new QPushButton(gameTable);                       // Create tile
            tile->setFixedSize(tileSize);                                         // Fix tile size
            tile->setText(QString::number(tileNames[vectorIndex]));               // Set a name
            tile->setFont(textFont);
            connect(tile, &QPushButton::clicked, this, &QPyat::tileMovement);     // Run movement on click

            gameLayout->addWidget(tile, 0+i, k);     // Add tile to the game table
            vectorIndex++;
        }
    }
}

void QPyat::tileMovement()
{
    QPushButton* tile = qobject_cast<QPushButton*>(sender());   // Take tile pointer from qobect_cast
    if(!tile) return;

    QPoint emptyPos = findEmpty(tile->pos());   // Check if there is an ampty tile
    tile->move(emptyPos.x(), emptyPos.y());     // Move tile
}

QPoint QPyat::findEmpty(const QPoint &tilePos)
{
    const int x {107};
    const int y {98};

    if(tilePos.x()-x > 0 && !gameTable->childAt(tilePos.x()-x, tilePos.y()+50)) return QPoint(tilePos.x()-x, tilePos.y());
    else if(tilePos.y()-x > 0 && !gameTable->childAt(tilePos.x(), tilePos.y()-50)) return QPoint(tilePos.x(), tilePos.y()-y);
    else if(tilePos.x()+x < 400 && !gameTable->childAt(tilePos.x()+x, tilePos.y()+50)) return QPoint(tilePos.x()+x, tilePos.y());
    else if(tilePos.y()+x < 400 && !gameTable->childAt(tilePos.x(), tilePos.y()+101)) return QPoint(tilePos.x(), tilePos.y()+y);
    return tilePos;
}


void QPyat::startNewGame()
{
    QList<QPushButton*> tilesList = gameTable->findChildren<QPushButton*>();
    for(auto tile: tilesList) delete tile;

    createPuzzle();
}
