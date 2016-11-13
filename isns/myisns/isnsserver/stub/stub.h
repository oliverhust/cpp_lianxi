#ifndef _STUB_H_
#define _STUB_H_

#ifdef  __cplusplus
extern "C"{
#endif

struct func_stub
{
    void *fn;
    char buf[5];
};

void stub_set(struct func_stub *stub, void *fn, void *fn_stub);
void stub_reset(struct func_stub *stub);

#ifdef __cplusplus
}
#endif

#endif /* _STUB_H_ */
