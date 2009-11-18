/********************************************************************
 * COPYRIGHT: 
 * Copyright (c) 2007-2008, International Business Machines Corporation and
 * others. All Rights Reserved.
 * Copyright (C) 2009, Yahoo! Inc.
 ********************************************************************/

#include "unicode/utypes.h"

#if !UCONFIG_NO_FORMATTING

#include "selfmts.h"
#include "cmemory.h"
#include "unicode/selfmt.h"
#include "stdio.h"

const UnicodeString SIMPLE_PATTERN = UnicodeString("feminine {feminineVerbValue} other{otherVerbValue}");
#define SELECT_PATTERN_DATA 4
#define SELECT_SYNTAX_DATA 7

void SelectFormatTest::runIndexedTest( int32_t index, UBool exec, const char* &name, char* /*par*/ )
{
    if (exec) logln("TestSuite SelectFormat");
    switch (index) {
        TESTCASE(0, selectFormatAPITest);
        TESTCASE(1, selectFormatUnitTest);
        default: name = "";
            break;
    }
}

/**
 * Unit tests of SelectFormat class.
 */
void SelectFormatTest::selectFormatUnitTest(/*char *par*/)
{
    UnicodeString patternTestData[SELECT_PATTERN_DATA] = {
        UNICODE_STRING_SIMPLE("fem {femValue} other{even}"),
        UNICODE_STRING_SIMPLE("other{odd or even}"),
        UNICODE_STRING_SIMPLE("odd{The number {0, number, integer} is odd.}other{The number {0, number, integer} is even.}"),
        UNICODE_STRING_SIMPLE("odd{The number {1} is odd}other{The number {1} is even}"),
    };

    UnicodeString checkSyntaxtData[SELECT_SYNTAX_DATA] = {
        UNICODE_STRING_SIMPLE("odd{foo} odd{bar} other{foobar}"),
        UNICODE_STRING_SIMPLE("odd{foo} other{bar} other{foobar}"),
        UNICODE_STRING_SIMPLE("odd{foo}"),
        UNICODE_STRING_SIMPLE("1odd{foo} other{bar}"),
        UNICODE_STRING_SIMPLE("odd{foo},other{bar}"),
        UNICODE_STRING_SIMPLE("od d{foo} other{bar}"),
        UNICODE_STRING_SIMPLE("odd{foo}{foobar}other{foo}"),
    };

    UErrorCode status = U_ZERO_ERROR;
    SelectFormat* selFmt = new SelectFormat( SIMPLE_PATTERN , status); 
    if (U_FAILURE(status)) {
        dataerrln("ERROR: SelectFormat Unit Test constructor failed in unit tests.- exitting");
        return;
    }
    
    // ======= Test SelectFormat pattern syntax.
    logln("SelectFormat Unit Test : Testing SelectFormat pattern syntax.");
    for (int32_t i=0; i<SELECT_SYNTAX_DATA; ++i) {
        status = U_ZERO_ERROR;
        selFmt->applyPattern(checkSyntaxtData[i], status);
        if (U_SUCCESS(status)) {
            errln("ERROR: SelectFormat Unit Test failed to detect syntax error with pattern: "+checkSyntaxtData[i]);
        }
    }

    status = U_ZERO_ERROR;
    selFmt = new SelectFormat( SIMPLE_PATTERN , status); 
    if (U_FAILURE(status)) {
        errln("ERROR: SelectFormat Unit Test constructor failed in unit tests.- exitting");
        return;
    }
    // ======= Test applying various pattern
    logln("SelectFormat Unit Test : Testing applying various patterns");
    
    for(int32_t i=0; i<SELECT_PATTERN_DATA; ++i) {
        status = U_ZERO_ERROR;
        selFmt->applyPattern(patternTestData[i], status);
        if (U_FAILURE(status)) {
            errln("ERROR: SelectFormat Unit Test failed to apply pattern- "+patternTestData[i] );
            continue;
        }
    }
    
    delete selFmt;
}

/**
 * Test various generic API methods of SelectFormat for Basic API usage.
 * This is to make sure the API test coverage is 100% .
 */
void SelectFormatTest::selectFormatAPITest(/*char *par*/)
{
    int numOfConstructors =3;
    UErrorCode status[3];
    SelectFormat* selFmt[3];

    // ========= Test constructors
    logln("SelectFormat API test: Testing SelectFormat constructors ...");
    for (int32_t i=0; i< numOfConstructors; ++i) {
        status[i] = U_ZERO_ERROR;
    }
    selFmt[0]= new SelectFormat(status[0]);
    if ( U_FAILURE(status[0]) ) {
        errln("ERROR: SelectFormat API test constructor with status failed!");
        return;
    }
    selFmt[1]= new SelectFormat(SIMPLE_PATTERN, status[1]);
    if ( U_FAILURE(status[1]) ) {
        errln("ERROR: SelectFormat API test constructor with pattern and status failed!");
        return;
    }

    // =========== Test copy constructor
    logln("SelectFormat API test: Testing copy constructor and == operator ...");
    SelectFormat fmt = *selFmt[1];
    SelectFormat* dupPFmt = new SelectFormat(fmt);
    if ((*selFmt[1]) != (*dupPFmt)) {
        errln("ERROR: SelectFormat API test Failed in copy constructor or == operator!");
    }
    delete dupPFmt;
    
    // ======= Test clone && == operator.
    logln("SelectFormat API test: Testing clone and == operator ...");
    if ( U_SUCCESS(status[0]) && U_SUCCESS(status[1]) ) {
        selFmt[1] = (SelectFormat*)selFmt[0]->clone();
        if (selFmt[1]!=NULL) {
            if ( *selFmt[1] != *selFmt[0] ) {
                errln("ERROR: SelectFormat API test clone test failed!");
            }
        }
    }

    // ======= Test assignment operator && == operator.
    logln("SelectFormat API test: Testing assignment operator and == operator ...");
    selFmt[2]= new SelectFormat(SIMPLE_PATTERN, status[2]);
    if ( U_SUCCESS(status[2]) ) {
        *selFmt[1] = *selFmt[2];
        if (selFmt[1]!=NULL) {
            if ( (*selFmt[1] != *selFmt[2]) ) {
                errln("ERROR: SelectFormat API test assignment operator test failed!");
            }
        }
        delete selFmt[1];
    }
    else {
         errln("ERROR: SelectFormat constructor failed in assignment operator!");
    }
    delete selFmt[0];
    delete selFmt[2];

    // ======= Test getStaticClassID() and getStaticClassID()
    logln("SelectFormat API test: Testing getStaticClassID() and getStaticClassID() ...");
    UErrorCode status1 = U_ZERO_ERROR;
    SelectFormat* selFmt1 = new SelectFormat( SIMPLE_PATTERN , status1); 
    if( U_FAILURE(status1)) {
        errln("ERROR: SelectFormat constructor failed in staticClassID test! Exitting");
        return;
    }

    logln("Testing getStaticClassID()");
    if(selFmt1->getDynamicClassID() !=SelectFormat::getStaticClassID()) {
        errln("ERROR: SelectFormat API test getDynamicClassID() didn't return the expected value");
    }

    // ======= Test applyPattern() and toPattern()
    logln("SelectFormat API test: Testing applyPattern() and toPattern() ...");
    UnicodeString pattern = UnicodeString("masculine{masculineVerbValue} other{otherVerbValue}");
    status1 = U_ZERO_ERROR;
    selFmt1->applyPattern( pattern, status1);
    if (U_FAILURE(status1)) {
        errln("ERROR: SelectFormat API test failed in applyPattern() with pattern: "+ pattern);
    }else{
        UnicodeString checkPattern;
        selFmt1->toPattern( checkPattern);
        if( checkPattern != pattern ){
            errln("ERROR: SelectFormat API test failed in toPattern() with unexpected result with pattern: "+ pattern);
        }
    }

    // ======= Test different format() methods 
    logln("SelectFormat API test: Testing  format() with keyword method ...");
    status1 = U_ZERO_ERROR;
    UnicodeString result;
    FieldPosition ignore(FieldPosition::DONT_CARE);
    UnicodeString keyWord = UnicodeString("masculine");

    selFmt1->format( keyWord, result , ignore , status1);
    if (U_FAILURE(status1)) {
        errln("ERROR: SelectFormat API test failed in format() with keyWord: "+ keyWord);
    }else{
        UnicodeString expected=UnicodeString("masculineVerbValue");
        if( result != expected ){
            errln("ERROR: SelectFormat API test failed in format() with unexpected result with keyWord: "+ keyWord);
        }
    }
    
    logln("SelectFormat API test: Testing  format() with Formattable obj method ...");
    status1 = U_ZERO_ERROR;
    result.remove();
    UnicodeString result1;
    Formattable testArgs = Formattable("other");
    selFmt1->format( testArgs, result1 , ignore , status1);
    if (U_FAILURE(status1)) {
        errln("ERROR: SelectFormat API test failed in format() with Formattable");
    }else{
        UnicodeString expected=UnicodeString("otherVerbValue");
        if( result1 != expected ){
            errln("ERROR: SelectFormat API test failed in format() with unexpected result with Formattable");
        }
    }

    delete selFmt1;
}
#endif /* #if !UCONFIG_NO_FORMATTING */
