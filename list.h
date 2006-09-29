#ifndef _list_h_
#define _list_h_

#include <stdlib.h>

typedef struct Node* List;

struct Node {
	void* info;
	List next;
};

List prependElement(List list, void* element);
List appendElement(List list, void* element);
List removeElement(List list, void* element);
List removeAll(List list);

#endif
