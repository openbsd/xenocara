#define SKIPS 8

#define G80DmaNext(pNv, data) \
     (pNv)->dmaBase[(pNv)->dmaCurrent++] = (data)

#define G80DmaStart(pNv, tag, size) {         \
     if((pNv)->dmaFree <= (size))             \
        G80DmaWait(pNv, size);                \
     G80DmaNext(pNv, ((size) << 18) | (tag)); \
     (pNv)->dmaFree -= ((size) + 1);          \
}

void G80DmaKickoff(G80Ptr pNv);
void G80DmaWait(G80Ptr pNv, int size);
