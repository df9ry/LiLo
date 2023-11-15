#include "about.hpp"
#include "ui_about.h"
#include "version.hpp"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);


    ui->textBrowser->setOpenLinks(true);
    ui->textBrowser->setOpenExternalLinks(true);
    ui->textBrowser->setHtml(
        "<h2>" APP_NAME "</h2>"
        "<table>"
            "<tr><td>Version:</td><td>" APP_VERSION "</td></tr>"
            "<tr><td>Copyright:</td><td>&copy; " APP_COPYRIGHT "</td></tr>"
            "<tr><td>Website:</td>"
                "<td><a href=\"" APP_WEBSITE "\">" APP_WEBSITE "</a></td>"
            "</tr>"
        "</table>"
    );
}

About::~About()
{
    delete ui;
}
