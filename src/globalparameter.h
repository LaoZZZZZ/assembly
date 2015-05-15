#ifndef GLOBALPARAMETER_H
#define GLOBALPARAMETER_H
#include <string>
#include <vector>
#include "mpi.h"
#include "mtpi_types.h"
using std::vector;
using std::string;

class globalParameter
{
public:
    globalParameter();
    int  kmerLength()const{return this->_klen;}
    int  prefixLength()const{return this->_prefixLen;}
    int  numberOfProcess()const{return this->_np;}
    vector<string>   inputfiles()const{return this->_inputfiles;}
    string           outputPrefix()const{return this->_outputPrefix;}
    int              readBlockSize()const{return this->_readBlocks;}
    MPI_Comm         comGroup(){return this->_group;}
    bool             RandomMapping()const{return this->_isRandom;}
    int              randomeSeed()const{return this->_seed;}
    void            setnp(int np){this->_np = np;}
    void            setPrefixLen(int plen){this->_prefixLen = plen;}
    void            setKmerLen(int klen){this->_klen = klen;}
    int             queuesize()const{return this->_queuesize;}
    int             maxTry()const{return this->_maxTry;}
    comMode         communicateMode(){return this->_communicationMode;}
    int             communicateThread(){return this->_comThread;}
    void            setCommunicationMode(comMode m){this->_communicationMode = m;}
    void            setCommunicationThread(int t){this->_comThread = t;}
private:
    void init(){
        _np = 1;
        _klen = 23;
        _prefixLen = 2;
        _readBlocks = 1800;
        _group = MPI_COMM_WORLD;
//        _queuesize = 65534;
        _queuesize = 1000000;
        _maxTry = 100;
        _comThread = 1;
        _communicationMode = BLOCKING;
    }
private:
    bool                                                _isRandom;
    int                                                 _seed;
    int                                                 _np;       // number of process
    int                                                 _klen;     // length of kmer
    int                                                 _prefixLen;  // prefix len
    std::vector<std::string>                            _inputfiles;  // input files;
    std::string                                         _outputPrefix;  //
    int                                                 _readBlocks; // size of read block at each reading
    MPI_Comm                                            _group;
    int                                                 _queuesize;
    int                                                 _maxTry;
    comMode                                             _communicationMode;
    int                                                 _comThread;
};

#endif // GLOBALPARAMETER_H
