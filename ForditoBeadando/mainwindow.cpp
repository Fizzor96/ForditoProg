#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->stack.push_back("#");
    this->stack.push_back("E");
    this->input = "";
    this->converted = "";
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if (this->ui->lineEdit->text() == "") {
        this->ui->status_lbl->setText(QString("Please enter a valid input!"));
    }
    else
    {
        this->ui->status_lbl->setText(QString(""));
    }

    input = this->ui->lineEdit->text();

    this->converted = this->input.replace(QRegularExpression("[0-9]"), "i");
    this->converted += "#";

    this->ui->lineEdit_2->setText(this->converted.trimmed().simplified());
}

void MainWindow::on_pushButton_2_clicked()
{
    if (this->converted == "") {
        this->ui->status_lbl->setText(QString("Please eneter a valid input!"));
        return;
    }
    if (this->ui->tableWidget->rowCount() == 0) {
        this->ui->status_lbl->setText(QString("Please load a csv!"));
        return;
    }
    this->converted = this->ui->lineEdit_2->text();

    this->stack.clear();
    this->ui->listWidget->clear();

    this->stack.push_back("#");
    this->stack.push_back("E");

    int inputindex = 0;
    while (true) {

        int stacksideindex = -1;
        for (int i = 0; i < this->ui->tableWidget->rowCount(); i++) {
            if(this->ui->tableWidget->item(i, 0) != nullptr || this->ui->tableWidget->item(i, 0) != NULL)
            {
                if (this->stack.back() == this->ui->tableWidget->item(i, 0)->text()) {
                    stacksideindex = i;
                    break;
                }
            }
        }

        int inputsideindex = -1;
        for (int i = 0; i < this->ui->tableWidget->columnCount(); i++) {
            if(this->ui->tableWidget->item(0, i) != nullptr || this->ui->tableWidget->item(0, i) != NULL)
            {
                if (this->converted[inputindex] == this->ui->tableWidget->item(0, i)->text()) {
                    inputsideindex = i;
                    break;
                }
            }
        }

        if (inputsideindex == -1 || stacksideindex == -1) {
            this->ui->status_lbl->setText("Invalid syntax table!");
            //std::cout << inputsideindex << " " << stacksideindex << std::endl;
            return;
        }

        if (this->ui->tableWidget->item(stacksideindex, inputsideindex) == nullptr ||
                this->ui->tableWidget->item(stacksideindex, inputsideindex) == NULL) {
            this->ui->status_lbl->setText("Error: empty cell invocation is not permitted!");
            //std::cout << "Error: empty cell invocation is not permitted!\n" << "inputsideindex: " << inputsideindex << " stacksideindex:" << stacksideindex << std::endl;
            break;
        }
        else if(this->ui->tableWidget->item(stacksideindex, inputsideindex)->text() == "accept")
        {
            this->ui->status_lbl->setText("Success!");
            break;
        }
        else if(this->ui->tableWidget->item(stacksideindex, inputsideindex)->text() == "pop")
        {
            this->stack.pop_back();
            inputindex++;
        }
        else
        {
            std::string raw = this->ui->tableWidget->item(stacksideindex, inputsideindex)->text().toStdString();
            raw.erase(std::remove(raw.begin(), raw.end(), '('), raw.end());
            raw.erase(std::remove(raw.begin(), raw.end(), ')'), raw.end());
            QString processed = QString(raw.c_str());
            //std::cout << processed.toStdString() << std::endl;
            QStringList cell = processed.split(",");
            //std::cout << cell[0].toStdString() << " " << cell[1].toStdString() << std::endl;
            this->stack.pop_back();
            for (int i = cell[0].length() - 1; i >= 0; i--) {

                if (cell[0] == "e") {
                    continue;
                }

                if(cell[0][i] == '\''){
                    QString sub;
                    int subindex = 0;
                    while(subindex < 2) {
                        sub.append(cell[0][i - 1 + subindex]);
                        subindex++;
                    }
                    //std::cout << sub.toStdString() << std::endl;
                    stack.push_back(sub);
                    i--;
                    continue;
                }

                this->stack.push_back(cell[0][i]);
                steps.push_back(cell[1]);
            }
            QStringList tempsteps;
            QString temp;
            temp += this->converted[inputindex];
            temp += "\t";
            temp += GetStack();
            temp += "\t";
            temp += GetSteps();
            tempsteps.push_back(temp);
            this->ui->listWidget->addItems(tempsteps);

        }
    }
}

QString MainWindow::GetSteps() const
{
    QString asd;
    for (unsigned int i = 0; i < this->steps.size(); i++) {
        asd.append(this->steps[i]);
    }
    return asd;
}

QString MainWindow::GetStack() const
{
    QString asd;
    for (unsigned int i = 0; i < this->stack.size(); i++) {
        asd.append(this->stack[i]);
    }
    return asd;
}


// Load .csv file into table
void MainWindow::on_pushButton_3_clicked()
{
    // Reset table
    this->ui->tableWidget->clear();
    if (this->ui->tableWidget->columnCount() > 0 || this->ui->tableWidget->rowCount() > 0) {
        this->ui->tableWidget->setColumnCount(0);
        this->ui->tableWidget->setRowCount(0);
    }

    // If can't open file or file does not exist return
    QFile file(ui->lineEdit_3->text());
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);

    // Have to parse our input to properly set table rows and cols
    unsigned int rowindex = 0;
    unsigned int colindex = 0;
    QString substr;

    bool initcols = false;
    while(!in.atEnd()){
        // inserting rows as long as we are reading lines
        this->ui->tableWidget->insertRow(this->ui->tableWidget->rowCount());

        QString line = in.readLine();

        // Init cols
        if (!initcols) {
            this->ui->tableWidget->insertColumn(this->ui->tableWidget->columnCount());
            for (unsigned int i = 0; i < line.length(); i++) {
                if (line[i] == ';') {
                    this->ui->tableWidget->insertColumn(this->ui->tableWidget->columnCount());
                }
            }
            initcols = true;
        }

        // Filling table with data
        for (unsigned int i = 0; i < line.length(); i++) {
            if (line[i] == ';'|| line[i] == '\n') {
                substr.clear();
                colindex++;
            }
            else
            {
                substr.append(line[i]);
            }
            this->ui->tableWidget->setItem(rowindex, colindex, new QTableWidgetItem(substr));
        }

        // reset colindex and rowindex
        substr.clear();
        colindex = 0;
        rowindex++;
    }
}


// Open .csv file dialog and set textfield.text to path of the .csv
void MainWindow::on_pushButton_4_clicked()
{
    QString file_path = QFileDialog::getOpenFileName(this, "Open Rules Table");
    ui->lineEdit_3->setText(file_path);
}






