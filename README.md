# ToySonic

This repository is a toy version of Services for Optimized Network Inference on Coprocessors (SONIC).
It exists solely to demonstrate the interface between an inference server and high energy physics experiment software that processes events in a loop.
The example workflow creates "Jets" that have different numbers of "constituents",
and then runs a "tagger" to generate a "discriminant" for each jet.
(These terms are in quotation marks because they are all simplified mockups with little physical meaning.)

The packages `Core`, `Formats`, and `Jets` are loosely based on [CMSSW](https://github.com/cms-sw/cmssw) (the CMS software).

The package `Sonic` is based on the implementation of SONIC in CMSSW: [SonicCore](https://github.com/cms-sw/cmssw/tree/master/HeterogeneousCore/SonicCore), [SonicTriton](https://github.com/cms-sw/cmssw/tree/master/HeterogeneousCore/SonicTriton).

The package `Infer` is based on the client classes of [Triton](https://github.com/triton-inference-server/client) (inference server).

## Usage

Compile:
```bash
./compile.sh
```

Run:
```bash
./run
```

Output:
```
Event 1:
        Jet 0: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        Jet 1: 1, 1, 1, 1, 1, 1, 1, 1, 
        Jet 2: 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        Jet 3: 3, 
        Jet 4: 4, 4, 
        Discrs: 0, 8, 20, 3, 8, 
Event 2:
        Jet 0: 0, 0, 
        Jet 1: 1, 
        Jet 2: 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        Jet 3: 3, 3, 3, 3, 3, 3, 
        Jet 4: 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 
        Discrs: 0, 1, 20, 18, 40, 
Event 3:
        Jet 0: 0, 
        Jet 1: 1, 1, 1, 1, 1, 1, 1, 1, 
        Jet 2: 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        Jet 3: 3, 3, 3, 
        Jet 4: 4, 4, 
        Jet 5: 5, 5, 5, 5, 5, 5, 
        Discrs: 0, 8, 18, 9, 8, 30, 
Event 4:
        Jet 0: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        Jet 1: 1, 1, 1, 1, 1, 1, 
        Jet 2: 2, 2, 
        Jet 3: 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 
        Jet 4: 4, 4, 4, 4, 4, 4, 4, 4, 4, 
        Jet 5: 5, 5, 5, 5, 5, 5, 5, 5, 
        Jet 6: 6, 6, 6, 6, 6, 6, 6, 
        Jet 7: 7, 7, 7, 7, 7, 7, 7, 
        Jet 8: 8, 8, 8, 8, 8, 8, 
        Jet 9: 9, 9, 9, 
        Discrs: 0, 6, 4, 30, 36, 40, 42, 49, 48, 27, 
Event 5:
        Jet 0: 0, 
        Jet 1: 1, 1, 1, 1, 1, 1, 1, 1, 1, 
        Jet 2: 2, 2, 2, 2, 2, 2, 2, 2, 2, 
        Discrs: 0, 9, 18, 
Event 6:
        Jet 0: 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        Jet 1: 1, 1, 1, 1, 
        Jet 2: 2, 2, 2, 
        Jet 3: 3, 3, 3, 3, 3, 3, 3, 3, 3, 
        Jet 4: 4, 
        Jet 5: 5, 
        Jet 6: 6, 6, 6, 6, 
        Jet 7: 7, 7, 
        Jet 8: 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 
        Discrs: 0, 4, 6, 27, 4, 5, 24, 14, 80, 
Event 7:
        Jet 0: 0, 0, 0, 
        Discrs: 0, 
Event 8:
        Jet 0: 0, 
        Jet 1: 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
        Jet 2: 2, 2, 2, 
        Jet 3: 3, 3, 3, 3, 3, 3, 3, 3, 3, 
        Jet 4: 4, 4, 4, 4, 
        Jet 5: 5, 5, 5, 5, 5, 5, 
        Jet 6: 6, 6, 6, 6, 6, 6, 6, 6, 6, 
        Jet 7: 7, 7, 7, 
        Jet 8: 8, 8, 8, 8, 8, 
        Discrs: 0, 10, 6, 27, 16, 30, 54, 21, 40, 
Event 9:
        Jet 0: 0, 0, 0, 0, 
        Discrs: 0, 
Event 10:
        Jet 0: 0, 0, 0, 
        Jet 1: 1, 
        Jet 2: 2, 2, 2, 2, 2, 
        Jet 3: 3, 3, 3, 3, 3, 3, 3, 
        Jet 4: 4, 4, 4, 4, 4, 
        Jet 5: 5, 5, 5, 5, 5, 5, 5, 5, 
        Jet 6: 6, 6, 6, 6, 6, 6, 6, 6, 6, 
        Jet 7: 7, 7, 7, 7, 7, 
        Discrs: 0, 1, 10, 21, 20, 40, 54, 35, 
```
