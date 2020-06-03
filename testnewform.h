#ifndef TESTNEWFORM_H
#define TESTNEWFORM_H

#include <QGroupBox>

namespace Ui
{
    class TestNewForm;
}

class TestNewForm : public QGroupBox
{
    Q_OBJECT

    public:
        // Constructor and destructor
            explicit TestNewForm(QWidget *parent = nullptr);
            ~TestNewForm();

    private:
        Ui::TestNewForm *ui;
};

#endif // TESTNEWFORM_H
