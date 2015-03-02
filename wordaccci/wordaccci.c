/**********************************************************************
 *
 *  wordaccci.c
 *
 *  Author: Stephen V. Rice
 *  
 * Copyright 1996 The Board of Regents of the Nevada System of Higher
 * Education, on behalf, of the University of Nevada, Las Vegas,
 * Information Science Research Institute
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License.  You
 * may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * permissions and limitations under the License.
 *
 **********************************************************************/

#include "util.h"
#include "ci.h"
#include "wacrpt.h"

#define usage  "[ -f report_files_list | wordacc_report1 wordacc_report2 ... ] >resultfile"

Wacdata wacdata;
Obslist obslist;

char *inputfilename;

Option option[] =
{
    {'f', &inputfilename, NULL},
    {'\0'}
};

/**********************************************************************/

void process_file(filename)
char *filename;
{
    long count, missed;
    count  = wacdata.total.count;
    missed = wacdata.total.missed;
    read_wacrpt(&wacdata, filename);
    append_obs(&obslist, wacdata.total.count - count,
    wacdata.total.missed - missed);
}
/**********************************************************************/

void write_results()
{
    double lower, upper;
    compute_ci(&obslist, &lower, &upper);
    printf("%14ld   Observations\n", obslist.count);
    printf("%14ld   Words\n", wacdata.total.count);
    printf("%14ld   Misrecognized\n", wacdata.total.missed);
    printf("%14.2f%%  Accuracy\n",
    100.0 * (wacdata.total.count - wacdata.total.missed) / wacdata.total.count);
    printf("%6.2f%%,%6.2f%%  %s\n", lower, upper,
    "Approximate 95% Confidence Interval for Accuracy");
}
/**********************************************************************/

int main(argc, argv)
int argc;
char *argv[];
{
    int i;
    initialize(&argc, argv, usage, option);
    if (argc < 2 && !inputfilename)
    error("not enough input files", Exit);

    process_all_files_from(inputfilename, &process_file);

    for (i = 0; i < argc; i++)
    process_file(argv[i]);
    write_results();
    return 0;
}
