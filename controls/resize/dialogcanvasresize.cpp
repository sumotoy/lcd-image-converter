/*
 * LCD Image Converter. Converts images and fonts for embedded applications.
 * Copyright (C) 2013 riuson
 * mailto: riuson@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/
 */

#include "dialogcanvasresize.h"
#include "ui_dialogcanvasresize.h"

#include <QItemSelectionModel>
#include "imagesmodel.h"
#include "imagesfilterproxy.h"
#include "resizesettings.h"
//-----------------------------------------------------------------------------
DialogCanvasResize::DialogCanvasResize(DataContainer *container, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCanvasResize)
{
    ui->setupUi(this);

    this->mContainer = container;

    this->mModel = new ImagesModel(container, Qt::Horizontal, this);

    this->mFilter = new ImagesFilterProxy(this);
    this->mFilter->setSourceModel(this->mModel);

    this->ui->tableView->setModel(this->mFilter);

    this->connect(this->ui->spinBoxLeft,   SIGNAL(valueChanged(int)), SLOT(spinBox_valueChanged(int)));
    this->connect(this->ui->spinBoxTop,    SIGNAL(valueChanged(int)), SLOT(spinBox_valueChanged(int)));
    this->connect(this->ui->spinBoxRight,  SIGNAL(valueChanged(int)), SLOT(spinBox_valueChanged(int)));
    this->connect(this->ui->spinBoxBottom, SIGNAL(valueChanged(int)), SLOT(spinBox_valueChanged(int)));

    this->mLeft = 0;
    this->mTop = 0;
    this->mRight = 0;
    this->mBottom = 0;

    int scale = ResizeSettings::scale();
    this->ui->spinBoxScale->setValue(scale);
    this->on_spinBoxScale_valueChanged(scale);
}
//-----------------------------------------------------------------------------
DialogCanvasResize::~DialogCanvasResize()
{
    ResizeSettings::setScale(this->ui->spinBoxScale->value());

    delete ui;
}
//-----------------------------------------------------------------------------
void DialogCanvasResize::selectKeys(const QStringList &keys)
{
    this->mFilter->setFilter(keys);

    this->resizeToContents();
}
//-----------------------------------------------------------------------------
void DialogCanvasResize::resizeInfo(int *left, int *top, int *right, int *bottom) const
{
    *left = this->mLeft;
    *top = this->mTop;
    *right = this->mRight;
    *bottom = this->mBottom;
}
//-----------------------------------------------------------------------------
void DialogCanvasResize::setResizeInfo(int left, int top, int right, int bottom)
{
    this->mLeft = left;
    this->mTop = top;
    this->mRight = right;
    this->mBottom = bottom;

    this->ui->spinBoxLeft->setValue(left);
    this->ui->spinBoxTop->setValue(top);
    this->ui->spinBoxRight->setValue(right);
    this->ui->spinBoxBottom->setValue(bottom);
}
//-----------------------------------------------------------------------------
void DialogCanvasResize::spinBox_valueChanged(int value)
{
    Q_UNUSED(value);

    this->mLeft = this->ui->spinBoxLeft->value();
    this->mTop = this->ui->spinBoxTop->value();
    this->mRight = this->ui->spinBoxRight->value();
    this->mBottom = this->ui->spinBoxBottom->value();

    this->mModel->setCrop(
                this->mLeft,
                this->mTop,
                this->mRight,
                this->mBottom);

    this->resizeToContents();
}
//-----------------------------------------------------------------------------
void DialogCanvasResize::on_spinBoxScale_valueChanged(int value)
{
    this->mModel->setScale(value);

    this->resizeToContents();
}
//-----------------------------------------------------------------------------
void DialogCanvasResize::on_pushButtonReset_clicked()
{
    this->ui->spinBoxLeft->setValue(0);
    this->ui->spinBoxTop->setValue(0);
    this->ui->spinBoxRight->setValue(0);
    this->ui->spinBoxBottom->setValue(0);
}
//-----------------------------------------------------------------------------
void DialogCanvasResize::resizeToContents()
{
    this->ui->tableView->resizeRowsToContents();
    this->ui->tableView->resizeColumnsToContents();
}
//-----------------------------------------------------------------------------
