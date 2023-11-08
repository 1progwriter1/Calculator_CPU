#ifndef LABELS_CPU
#define LABELS_CPU

struct Label {
    char *name;
    int address;
    int *addresses_to_fill;
    int size = 1;
    int current;
};

struct Labels {
    Label *data;
    int current;
    int size;
};


const int NO_ADDRESS = -1;

int SetLabel(Labels *lbls, char *name, int current_address, int address);
int LabelsCtor(Labels *lbls);
int LabelsDtor(Labels *lbls);

#endif