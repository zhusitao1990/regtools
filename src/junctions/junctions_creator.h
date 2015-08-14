/*  junctions_creator.h -- Declarations for `junctions create` command

    Copyright (c) 2015, The Griffith Lab

    Author: Avinash Ramu <aramu@genome.wustl.edu>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.  */

#ifndef JUNCTIONS_CREATOR_H
#define JUNCTIONS_CREATOR_H

#include <iostream>
#include "bedFile.h"
#include "sam.h"
//#include "faidx.h"
//#include "kstring.h"

using namespace std;

//Format of an junction
struct Junction : BED {
    unsigned int read_count;
    CHRPOS thick_start;
    CHRPOS thick_end;
    //Has the junction been added to the map
    bool added;
    //Does the junction satisfy the min anchor requirement
    //Atleast one read containing the junction needs to have
    // min anchor on both sides.
    bool has_min_anchor;
    Junction() {
        start = 0;
        end = 0;
        thick_start = 0;
        thick_end = 0;
        read_count = 0;
        added = false;
        has_min_anchor = false;
    }
};

//The class that deals with creating the junctions
class JunctionsCreator {
    private:
        //Alignment file
        string bam_;
        //Minimum anchor length for junctions
        //Junctions need atleast this many bp overlap
        // on both ends.
        int min_anchor_length;
        //Map to store the junctions
        //The key is "chr:start-end"
        //The value is an object of type Junction(see above)
        map<string, Junction> junctions;
        //File to write output to - optional, write to STDOUT by default
        string output_file;
        //Pull out the cigar string from the read
        int parse_read(bam_hdr_t *header, bam1_t *aln);
        //Parse junctions from the read and store in junction map
        int parse_cigar_into_junctions(string chr, int read_pos,
                                       uint32_t *cigar, int n_cigar);
        //Add a junction to the junctions map
        int add_junction(Junction j1);
        //Get the strand from the XS aux tag
        void set_junction_strand(bam1_t *aln, Junction& j1);
    public:
        //Default constructor
        JunctionsCreator() {
            min_anchor_length = 8;
        };
        //Parse command-line options for this tool
        int parse_options(int argc, char *argv[]);
        //Print default usage
        int usage(ostream& out = cerr);
        //Identify exon-exon junctions
        int identify_junctions_from_BAM();
        //Print one junction
        void print_one_junction(const Junction j1, ostream& out = cout);
        //Print all the junctions
        void print_all_junctions(ostream& out = cout);
        //Get the BAM filename
        string get_bam();
        //Check if a junction satisfies minimum anchor property
        bool properly_anchored(Junction j1);
        //Parse the alignment into the junctions map
        int parse_alignment_into_junctions(bam_hdr_t *header, bam1_t *aln);
};

#endif
