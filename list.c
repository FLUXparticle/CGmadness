#include "list.h"

#include "debug.h"

/*
 * Ein einfaches Modul für eine dynamische Listen-Struktur
 */

List newNode(void* element, List next) {
	List result;
	MALLOC(result, sizeof(*result));
	if (!result) {
		/* ERROR */
	}
	result->info = element;
	result->next = next;
	return result;
}

List prependElement(List list, void* element) {
	return newNode(element, list);
}

List appendElement(List list, void* element) {
	if (!list) {
		return newNode(element, list);
	}

	list->next = appendElement(list->next, element);
	return list;
}

List removeHead(List list) {
	List tail;
	
	if (!list) {
		return list;
	}

	tail = list->next;
	FREE(list);
	return tail;
}

List removeElement(List list, void* element) {
	if (!list) {
		return list;
	}

	if (list->info == element) {
		return removeHead(list);
	}

	list->next = removeElement(list->next, element);
	return list;
}

List removeAll(List list) {
	while (list) {
		list = removeHead(list);
	}

	return list;
}
