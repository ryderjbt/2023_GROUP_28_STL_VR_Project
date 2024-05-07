/** @file
* declarations of all functions and member variables used by the OptionDialog class
*/

#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>

namespace Ui {
    class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:

    /** Constructor
     *  This will construct an OptionDialog object, and set up all nessecary interactable features
     * @param *parent is a pointer to a QWidget, if no object is provided this will be set to nullptr
     */

    explicit OptionDialog(QWidget *parent = nullptr);

    /** Destructor
    * Deletes the OptionDialog ui object
    */

    ~OptionDialog();

    /**Set the name of the selected part
    * @param name is the user inputted name for the selected part
    */

    void setName(QString name);

    /**Get the name of the selected part
    * Returns name of selected part so that it is displayed correctly
    */

    QString getName();

    /**Sets the sliders to represent current colour of the selected part
    * @param red is the value of red in the cars current colour (0-255)
    * @param green is the value of green in the cars current colour (0-255)
    * @param blue is the value of blue in the cars current colour (0-255)
    */

    void setRGB(unsigned int red, unsigned int green, unsigned int blue);

    /**Returns current slider values for red colour
    */

    unsigned int getR();

    /**Returns current slider values for green colour
    */

    unsigned int getG();

    /**Returns current slider values for blue colour
    */

    unsigned int getB();

    /**Sets visibility slider to current amount held by the selected part
    * @param visibility is current value (0.0-1.0) of selected part's visibility/opacity
    */

    void setVisibleDialog(double visibility);

    /**Returns current slider value of visibility, so selected parts can be updated
    */

    double getVisible();

private slots:

    /**Updates red colour to current slider amount and represents it in the Qlabel colour square
    */

    void changeR();

    /**Updates green colour to current slider amount and represents it in the Qlabel colour square
    */

    void changeG();

    /**Updates blue colour to current slider amount and represents it in the Qlabel colour square
    */

    void changeB();

private:
    Ui::OptionDialog *ui;
    QString r;  
    QString g;
    QString b;
};

#endif // OPTIONDIALOG_H
