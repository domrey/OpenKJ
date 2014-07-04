/*
 * Copyright (c) 2013-2014 Thomas Isaac Lightburn
 *
 *
 * This file is part of OpenKJ.
 *
 * OpenKJ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "databasedialog.h"
#include "ui_databasedialog.h"
#include "databaseupdatethread.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>

DatabaseDialog::DatabaseDialog(QSqlDatabase *db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatabaseDialog)
{
    m_db = db;
    ui->setupUi(this);
    pathsModel = new QSqlTableModel(this, *db);
    pathsModel->setTable("srcdirs");
    pathsModel->select();
    ui->tableViewPaths->setModel(pathsModel);
    pathsModel->sort(0, Qt::AscendingOrder);
    selectedDirectoryIdx = -1;
}

DatabaseDialog::~DatabaseDialog()
{
    delete ui;
}

void DatabaseDialog::on_pushButtonAdd_clicked()
{
    QString fileName = QFileDialog::getExistingDirectory(this);
    if (fileName != "")
    {
        pathsModel->insertRow(pathsModel->rowCount());
        pathsModel->setData(pathsModel->index(pathsModel->rowCount() - 1, 0), fileName);
        pathsModel->submitAll();
    }
}

void DatabaseDialog::on_pushButtonUpdate_clicked()
{
    if (selectedDirectoryIdx >= 0)
    {
        QMessageBox *msgBox = new QMessageBox(this);
        msgBox->setStandardButtons(0);
        msgBox->setText("Updating Database, please wait...");
        msgBox->show();

        DatabaseUpdateThread thread;
        thread.setPath(pathsModel->data(pathsModel->index(selectedDirectoryIdx, 0)).toString());
        qDebug() << "Updating for path: " << pathsModel->data(pathsModel->index(selectedDirectoryIdx, 0)).toString();
        thread.start();
        while (thread.isRunning())
            QApplication::processEvents();

        msgBox->close();
        delete msgBox;
    }
}

void DatabaseDialog::on_pushButtonUpdateAll_clicked()
{
    QMessageBox *msgBox = new QMessageBox(this);
    msgBox->setStandardButtons(0);
    msgBox->setText("Updating Database, please wait...");
    msgBox->show();
    for (int i=0; i < pathsModel->rowCount(); i++)
    {
        QApplication::processEvents();
        DatabaseUpdateThread thread;
        thread.setPath(pathsModel->data(pathsModel->index(i, 0)).toString());
        thread.start();
        while (thread.isRunning())
            QApplication::processEvents();
    }
    msgBox->close();
    delete msgBox;
}

void DatabaseDialog::on_pushButtonClose_clicked()
{
    close();
}

void DatabaseDialog::on_pushButtonClearDb_clicked()
{
    QSqlQuery query;
    query.exec("DELETE FROM playlists");
    query.exec("DELETE FROM plsongs");
    query.exec("DELETE FROM songs");
    query.exec("VACUUM");
}

void DatabaseDialog::on_pushButtonDelete_clicked()
{
    pathsModel->removeRow(selectedDirectoryIdx);
    pathsModel->select();
    pathsModel->submitAll();
}

void DatabaseDialog::on_tableViewPaths_clicked(const QModelIndex &index)
{
    if (index.row() >= 0)
        selectedDirectoryIdx = index.row();
    else
        selectedDirectoryIdx = -1;
    qDebug() << "Selected path row" << selectedDirectoryIdx;
}


