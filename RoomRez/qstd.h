#ifndef QSTD_H
#define QSTD_H

#include <QTextStream>
#include <QFile>
#include <QString>
#include "utils_export.h"

/** @short helper objects and functions which help reduce the
       need for char[] and the standard library.  
    
    Also provided, some helper functions for writing
    interactive stdin/stdout applications.
	@author Alan Ezust
*/
//start
namespace qstd {

    /** yes/no prompt
        interactive stdin UI - prompts user with
        a yes/no question. Repeatedly-asks
        until user supplies a valid answer.
 
        @param yesNoQuestion the yes/no question
        @return true/false depending on what the
        user responded.
    */
     UTILS_EXPORT bool yes(QString yesNoQuestion);
    /** Convenience function that feeds a  specific question
        to the yes() function.
        @usage do {.....} while(more ("foobar"));
        so that user sees the question: "Another foobar (y/n)? "
        @param name of the item being handled by the loop.
    */
     UTILS_EXPORT  bool more(QString prompt);
    /** A function for safely taking an int from the keyboard.
        Takes data into a  QString and tests to make sure it
        can be converted to int before returning.
        @param base allows choice of number base.
        @return returns validated int.
    */
     UTILS_EXPORT int promptInt(int base = 10);
    /** A function for safely taking a double from the keyboard.
        Takes data into a  QString and tests to make sure it
        can be converted to double before returning.
        @return returns validated int.
    */
     UTILS_EXPORT double promptDouble();
    /** Complete dialog for opening a file for output.
        Asks user for file name, checks to see if
        file already exists and, if so, asks the user if
        it is ok to overwrite.
        @param Reference QFile parameter is set to point
        to the (eventually) opened file.
    */
    /** @short Dialog for a output file prompt
     */
     UTILS_EXPORT void promptOutputFile(QFile& outfile);

    /** @short Dialog for input file prompt */
     UTILS_EXPORT void promptInputFile(QFile& infile);
//end
};

#endif

