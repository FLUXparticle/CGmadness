/*
 * CG Madness - a Marble Madness clone
 * Copyright (C) 2007  Sven Reinck <sreinck@gmx.de>
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "list.h"

#include "debug.h"

/*
 * a simple module for a linked list
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

List removeElement(List list, const void* element) {
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

int countElements(const List list) {
  if (!list) {
    return 0;
  }
  
  return 1 + countElements(list->next);
}

void* getElement(const List list, int index) {
  if (!list) {
    return NULL;
  }
  
  if (index == 0) {
    return list->info;
  }
  
  return getElement(list->next, index - 1);
}
