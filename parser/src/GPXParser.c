/*
*	Saarthi Baluja #1098654
*	GPX file parser API
*/

#include <stdio.h>
#include <string.h>
#include "LinkedListAPI.h"
#include "GPXParser.h"
#include <libxml/parser.h>
#include <libxml/tree.h>


char* GPXdocToString(GPXdoc *mydoc){
	if (mydoc == NULL){
		return NULL;
	}
	char *wpts = toString(mydoc->waypoints);
	char *rtes = toString(mydoc->routes);
	char *trks = toString(mydoc->tracks);

	char *finalString = malloc(sizeof(char) * ( strlen(wpts) + strlen(rtes) + strlen(trks)  + 1));

	strcpy(finalString, wpts);
	strcat(finalString, rtes);
	strcat(finalString, trks);
	free(wpts);
	free(rtes);
	free(trks);
	return finalString;
}

void deleteGPXdoc(GPXdoc *mydoc){
	if (mydoc == NULL){
		return;
	}
	free(mydoc->creator);
	freeList(mydoc->waypoints);
	freeList(mydoc->routes);
	freeList(mydoc->tracks);
	free(mydoc);
}

int compareWaypoints(const void *ptr, const void *ptr2){
    return 1;
}

char* waypointToString(void *data){
	if (data == NULL){
		return NULL;
	}

	Waypoint *wpt = (Waypoint*)data;
	char *name;
	char *latitude = malloc(sizeof(char)*20);
	char *longitude = malloc(sizeof(char)*20);
	char *otherData = toString(wpt->otherData);
	sprintf(latitude, "%f", wpt->latitude);
	sprintf(longitude, "%f", wpt->longitude);
	name = malloc(sizeof(char)*(strlen(wpt->name) + 1 + strlen(latitude) + 1 + strlen(longitude) + strlen(otherData) + 100));
	sprintf(name, "WPT: %s: Latitude = %s Longitude = %s\n", wpt->name, latitude, longitude);
	strcat(name, "\tWPT OTHERDATA:\n");
	strcat(name, otherData);
	strcat(name, "\n\tEND OF OTHERDATA\n");
	strcat(name, "END OF WPT\n");
	free(latitude);
	free(longitude);
	free(otherData);
	return name;
}

void deleteWaypoint(void *data){
    if (data == NULL){
		return;
	}

	Waypoint *wpt = (Waypoint*)data;
	freeList(wpt->otherData);
	free(wpt->name);
	free(wpt);
}

int compareRoutes(const void *ptr, const void *ptr2){
    return 1;
}

char* routeToString(void *data){
	if (data == NULL){
		return NULL;
	}

	Route *rte = (Route*)data;
	char *wpts = toString(rte->waypoints);
	char *otherdata = toString(rte->otherData);
	char *string = malloc(sizeof(char) * (strlen(rte->name) + strlen(wpts) + 100 + strlen(otherdata)));
	strcpy(string, "\nROUTE:\n");
	strcat(string, rte->name);
	strcat(string, wpts);
	strcat(string, "\nROUTE OTHER DATA:\n");
	strcat(string, otherdata);
	strcat(string, "\nEND OF OTHER ROUTE DATA\n");
	strcat(string, "\nEND OF ROUTE\n");
	free(wpts);
	free(otherdata);
	return string;
	
}

void deleteRoute(void *data){
	if (data == NULL){
		return;
	}
	Route *rte = (Route*)data;
	freeList(rte->waypoints);
	freeList(rte->otherData);
	free(rte->name);
	free(rte);
}

int compareTracks(const void *ptr, const void *ptr2){
    return 1;
}

char* trackToString(void *data){
	if (data == NULL){
		return NULL;
	}
	Track *trk = (Track*)data;
	char *trksgs = toString(trk->segments);
	char *otherData = toString(trk->otherData);
	char *string = malloc(sizeof(char) * (strlen(trk->name) + strlen(trksgs) + 100 + strlen(otherData)));
	strcpy(string, "\nTRACK:");
	strcat(string, trk->name);
	strcat(string, "\n");
	strcat(string, trksgs);
	strcat(string, "\nTRACK OTHER DATA:\n");
	strcat(string, otherData);
	strcat(string, "\nEND OF TRACK OTHER DATA\n");
	strcat(string, "\nEND OF TRACK\n");
	free(trksgs);
	free(otherData);
	return string;
}

void deleteTrack(void *data){
	if (data == NULL){
		return;
	}
	Track *trk = (Track*)data;
	freeList(trk->segments);
	freeList(trk->otherData);
	free(trk->name);
	free(trk);
}

int compareTrackSegments(const void *ptr, const void *ptr2){
    return 1;
}

char* trackSegmentToString(void *data){
	if (data == NULL){
		return NULL;
	}
	TrackSegment *trksg = (TrackSegment*)data;
	char *wpts = toString(trksg->waypoints);
	char *string = malloc(sizeof(char) * strlen(wpts) + 39);
	strcpy(string, "\nTRACK SEGMENT:\n");
	strcat(string, wpts);
	strcat(string, "\nEND OF TRACK SEGMENT");
	free(wpts);
	return string;
}

void deleteTrackSegment(void *data){
	if (data == NULL){
		return;
	}
	TrackSegment *trksg = (TrackSegment*)data;
	freeList(trksg->waypoints);
	free(trksg);
}

int compareGpxData(const void *ptr, const void *ptr2){
    return 1;
}

char* gpxDataToString(void *data){
	if (data == NULL){
		return NULL;
	}
	GPXData *otherData = (GPXData*)data;
	char *string = malloc (sizeof(char) * (strlen(otherData->name) + strlen(otherData->value)) + 100);
	sprintf(string, "NAME: %s\t VALUE: %s\n", otherData->name, otherData->value);
	return string;
}

void deleteGpxData(void *data){
	if (data == NULL){
		return;
	}
	GPXData *otherData = (GPXData*)data;
	free(otherData);
}

void dummyDelete(void *data){
	return;
}

GPXdoc* initGPXdoc(xmlNode *gpx_node){
    GPXdoc *init = malloc(sizeof(GPXdoc));
    
    strcpy(init->namespace,"saarthi");
    init->version = 1.1;
	init->creator = malloc(sizeof(char) * strlen((char*)gpx_node->ns->href) + 1);
    strcpy(init->creator,(char*)gpx_node->ns->href);
	strcpy(init->namespace,(char*)gpx_node->ns->href);
	//printf("%s", init->creator);

    if (!init){
        return NULL;
    }

    xmlAttr *attr;
        for (attr = gpx_node->properties; attr != NULL; attr = attr->next)
        {
            xmlNode *value = attr->children;
            char *attrName = (char *)attr->name;
            char *cont = (char *)(value->content);
            if (strcmp(attrName, "version") == 0){
                init->version = strtod(cont, NULL);
            } else if (strcmp(attrName, "creator") == 0){
				init->creator = realloc(init->creator, sizeof(char)*strlen(cont) + 2);
                if (init->creator){
                    strcpy(init->creator, cont);
                }
            } 
        }
    
    init->waypoints = initializeList(&waypointToString, &deleteWaypoint, &compareWaypoints);
    init->routes = initializeList(&routeToString, &deleteRoute, &compareRoutes);
    init->tracks = initializeList(&trackToString, &deleteTrack, &compareTracks);
    return init;
}

void createWayPoint(xmlNode *wpt_node, List *waypoints){
     xmlAttr *attr;
	 Waypoint *newwpt = malloc(sizeof(Waypoint));
     newwpt->otherData = initializeList(&gpxDataToString, &deleteGpxData, &compareGpxData);
     newwpt->latitude = -1;
     newwpt->longitude = -1;
     newwpt->name = malloc(sizeof(char) * 1);
     strcpy(newwpt->name, "\0");
     
     if (!newwpt){
         return;
     }
        for (attr = wpt_node->properties; attr != NULL; attr = attr->next)
        {
            xmlNode *value = attr->children;
			//printf("ATTR:%s\tCONT%f\n", newwpt->name, newwpt->latitude);
            char *attrName = (char *)attr->name;
            char *cont = (char *)(value->content);
            if (strcmp(attrName, "lat") == 0){
                newwpt->latitude = strtod(cont, NULL);
            } else if (strcmp(attrName, "lon") == 0){
                newwpt->longitude = strtod(cont, NULL);
            } 
            if(newwpt->latitude != -1 && newwpt->longitude != -1){
                //printf("ATTR:%f\t\tCT%f\t LOC:%p\n", newwpt->latitude, newwpt->longitude, &newwpt);
                xmlNode *children = NULL;
                

                for (children = wpt_node->children; children != NULL; children = children->next){

					if (children->type == XML_ELEMENT_NODE){

					

						//printf("FOund loop\n");
			
						if(strcmp((char*)children->name, "name") == 0){

						//	printf("FOund name\n");
	
							if (children->children->content != NULL){
								char *cont2 = (char*)children->children->content;	
								newwpt->name = realloc(newwpt->name, sizeof(char)*strlen(cont2) + 1);
								strcpy(newwpt->name,cont2);
								//printf("%s\n", newwpt->name);
							}
						}  else {
							GPXData *otherData = malloc(sizeof(GPXData) + sizeof(char));
							strcpy(otherData->name, (char*)children->name);
							strcpy(otherData->value, "\0");
							//printf("%s: %s\n", otherData->name, otherData->value);
							if (children->children->content != NULL){
								char *cont2 = (char*)children->children->content;
								if (strlen(cont2) > 1){

									otherData = realloc(otherData, sizeof(GPXData) + (strlen(cont2) + 2 * sizeof(char)));
									if (otherData){
										strcpy(otherData->value,cont2);
									}

								}
							}
							//printf("%s: %s\n", otherData->name, otherData->value);
							insertBack(newwpt->otherData, otherData);
						}
                }
				}
                insertBack(waypoints, (void*)newwpt);   
            }
        }
}

void createRoute(xmlNode *rte_node, GPXdoc *mydoc){
	xmlNode *cur_node = NULL;
	Route *rte = malloc(sizeof(Route));
	rte->waypoints = initializeList(&waypointToString, &deleteWaypoint, &compareWaypoints);
	rte->otherData = initializeList(&gpxDataToString, &deleteGpxData, &compareGpxData);
	rte->name = malloc(sizeof(char) * 1);
    strcpy(rte->name, "\0");
	char *cont;

	for (cur_node = rte_node->children; cur_node != NULL; cur_node = cur_node->next){
		if (cur_node->type == XML_ELEMENT_NODE){

//			printf("%s\n", (char*)cur_node->name);

			if(strcmp((char*)cur_node->name, "name") == 0){

				if(cur_node->children->content != NULL){
					cont = (char*)cur_node->children->content;
					rte->name = realloc(rte->name, sizeof(char)* strlen(cont) + 1);
					strcpy(rte->name, cont);
					//printf("%s\n", rte->name);
				}
			} else if (strcmp((char*)cur_node->name, "rtept") == 0) {
				createWayPoint(cur_node, rte->waypoints);
			} else {
				GPXData *otherData = malloc(sizeof(GPXData) + sizeof(char));
				strcpy(otherData->value, "\0");
				strcpy(otherData->name, (char*)cur_node->name);
				if (cur_node->children->content){
					char *cont2 = (char*)cur_node->children->content;
					otherData = realloc(otherData, sizeof(GPXData) + strlen(cont2) + 1 * sizeof(char));
						if (otherData){
							strcpy(otherData->value,cont2);
						}
					//printf("%s\t %s\n", otherData->name, otherData->value);
				}
				insertBack(rte->otherData, otherData);
			}
		}
	}
	insertBack(mydoc->routes, (void*)rte);

}

void createTrackSegment(xmlNode *trkseg_node, List *toAdd){
	xmlNode *cur_node = NULL;
	TrackSegment *seg = malloc(sizeof(TrackSegment));
	seg->waypoints = initializeList(&waypointToString, &deleteWaypoint, &compareWaypoints);

	for (cur_node = trkseg_node->children; cur_node != NULL; cur_node = cur_node->next){
		if(cur_node->type == XML_ELEMENT_NODE){
			
			if((strcmp((char*)cur_node->name, "trkpt")) == 0){
				createWayPoint(cur_node, seg->waypoints);
			}
		}
	}
	insertBack(toAdd, (void*)seg);
}
void createTrack(xmlNode *trk_node, GPXdoc *mydoc){
	xmlNode *cur_node = NULL;
	Track *trk = malloc(sizeof(Track));
	char *cont;
	trk->name = malloc(sizeof(char) * 1);
	strcpy(trk->name, "\0");
	trk->otherData = initializeList(&gpxDataToString, &deleteGpxData, &compareGpxData);
	trk->segments = initializeList(&trackSegmentToString, &deleteTrackSegment, &compareTrackSegments);


	for(cur_node = trk_node->children; cur_node != NULL; cur_node = cur_node->next){
		if (cur_node->type == XML_ELEMENT_NODE){

			if(strcmp((char*)cur_node->name, "name") == 0){

				if(cur_node->children->content != NULL){
					cont = (char*)cur_node->children->content;
					trk->name = realloc(trk->name,sizeof(char)* strlen(cont) + 1);
					strcpy(trk->name, cont);
					//printf("%s\n", trk->name);
				}
			} else if (strcmp((char*)cur_node->name, "trkseg") == 0) {
				
				createTrackSegment(cur_node, trk->segments);
			} else {
				GPXData *otherData = malloc(sizeof(GPXData) + sizeof(char));
				strcpy(otherData->name, (char*)cur_node->name);
				strcpy(otherData->value, "\0");
				//printf("OTHERDATA:%s\n", otherData->name);
				if (cur_node->children->content != NULL){
					char *cont2 = (char*)cur_node->children->content;
					otherData = realloc(otherData, sizeof(GPXData) + strlen(cont2) + 1 * sizeof(char));
					if (otherData){
						//printf("worked?\n");
						strcpy(otherData->value, (char*)cur_node->children->content);
						//printf("OTHERDATA CONTENT:%s\n", otherData->value);
					}
				}
				insertBack(trk->otherData, otherData);
			}

		}

	}
	insertBack(mydoc->tracks, trk);



}

void populate(xmlNode *a_node, GPXdoc **parsed){

    xmlNode *cur_node = NULL;

    for (cur_node = a_node; cur_node != NULL; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if(strcmp((char*)cur_node->name, "gpx") == 0){
                *parsed = initGPXdoc(cur_node);
            } else if (strcmp((char*)cur_node->name, "wpt") == 0 && *parsed != NULL){
                createWayPoint(cur_node, (*parsed)->waypoints);
            } else if (strcmp((char*)cur_node->name, "rte") == 0 && *parsed != NULL){
                createRoute(cur_node, *parsed);
            } else if (strcmp((char*)cur_node->name, "trk") == 0 && *parsed != NULL){
                createTrack(cur_node, *parsed);
            }
        }

         if (cur_node->content != NULL ){
             //printf("  content: %s\n", cur_node->content);
         }

        if (parsed && cur_node->children){
            populate(cur_node->children, parsed);
        }   
    }

}

GPXdoc* createGPXdoc(char* fileName){
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;
    GPXdoc *parsed = NULL;

	LIBXML_TEST_VERSION

    doc = xmlReadFile(fileName, NULL, 0);
    if (doc == NULL) {
        return NULL;
    }

    root_element = xmlDocGetRootElement(doc);

    populate(root_element, &parsed);

	xmlFreeDoc(doc);
    xmlCleanupParser();

    return parsed;
}

GPXdoc* createValidGPXdoc(char* fileName,  char* gpxSchemaFile){
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;
    GPXdoc *parsed = NULL;
	xmlSchemaPtr schema = NULL;
	xmlSchemaParserCtxtPtr ctxt;
	int ret;
	
	xmlLineNumbersDefault(1);

	ctxt = xmlSchemaNewParserCtxt(gpxSchemaFile);

	xmlSchemaSetParserErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
	schema = xmlSchemaParse(ctxt);
	xmlSchemaFreeParserCtxt(ctxt);

	doc = xmlReadFile(fileName, NULL, 0);

	LIBXML_TEST_VERSION
	
    if (doc == NULL) {
		xmlSchemaCleanupTypes();
		xmlCleanupParser();
		xmlMemoryDump();
        return NULL;
    } else {
		xmlSchemaValidCtxtPtr ctxt;
 
		ctxt = xmlSchemaNewValidCtxt(schema);
		xmlSchemaSetValidErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
		ret = xmlSchemaValidateDoc(ctxt, doc);

		if(ret == 0){
			root_element = xmlDocGetRootElement(doc);
    		populate(root_element, &parsed);
			xmlSchemaFreeValidCtxt(ctxt);
			xmlFreeDoc(doc);
		} else {
			xmlSchemaFreeValidCtxt(ctxt);
			xmlFreeDoc(doc);
			return NULL;
		}
	}	

	if(schema != NULL){
		xmlSchemaFree(schema);
	}

	xmlSchemaCleanupTypes();
	xmlCleanupParser();
	xmlMemoryDump();
	
	return parsed;
}

bool validateWaypoint(Waypoint *wpt){
	if(wpt->name == NULL){
		return false;
	}
	if(wpt->longitude > 180 || wpt->longitude < -180){
		return false;
	}
	if(wpt->latitude > 90 || wpt->latitude < -90){
		return false;
	}
	if(wpt->otherData == NULL){
		return false;
	}
	return true;
}

bool validateRoute(Route *rte){
	if (rte->name == NULL){
		return false;
	}
	if(rte->waypoints == NULL || rte->otherData == NULL){
		return false;
	}
	ListIterator wptIter = createIterator(rte->waypoints);
	void *data;
	while((data = nextElement(&wptIter))){
		Waypoint *wpt = (Waypoint*)data;
		if(validateWaypoint(wpt) == false){
			return false;
		}
	}
	return true;
}

bool validateSegment(TrackSegment *trkseg){
	if(trkseg->waypoints == NULL){
		return false;
	}
	ListIterator wptIter = createIterator(trkseg->waypoints);
	void *data;
	while((data = nextElement(&wptIter))){
		Waypoint *wpt = (Waypoint*)data;
		if(validateWaypoint(wpt) == false){
			return false;
		}
	}
	return true;
}

bool validateTrack(Track *trk){
	if(trk->name == NULL){
		return false;
	}
	if(trk->segments == NULL || trk->otherData == NULL){
		return false;
	}
	ListIterator segIter = createIterator(trk->segments);
	void *data;
	while((data = nextElement(&segIter))){
		TrackSegment *trkseg = (TrackSegment*)data;
		if(validateSegment(trkseg) == false){
			return false;
		}
	}
	return true;
}

bool validateGPXHeader(GPXdoc* doc){
		if(doc == NULL){
			return false;
		}

		
	if (strlen(doc->namespace) == 0){
		return false;
	}
	if (doc->creator == NULL || strlen(doc->creator) == 0){
		return false;
	}
	if(doc->version < 0){
		return false;
	}
	if(doc->waypoints == NULL || doc->routes == NULL || doc->tracks == NULL){
		return false;
	}

	ListIterator wptIter = createIterator(doc->waypoints);
	ListIterator rteIter = createIterator(doc->routes);
	ListIterator trkIter = createIterator(doc->tracks);

	void *data;
	while ((data = nextElement(&wptIter))){
		Waypoint *wpt = (Waypoint*)data;
		if(validateWaypoint(wpt) == false){
			return false;
		}
	}
	while((data = nextElement(&rteIter))){
		Route *rte = (Route*)data;
		if(validateRoute(rte) == false){
			return false;
		}
	}
	while((data = nextElement(&trkIter))){
		Track *trk = (Track*)data;
		if(validateTrack(trk) == false){
			return false;
		}
	}
	return true;
}



xmlNodePtr WaypointToXml(Waypoint *wpt, xmlNsPtr ns, int flag){
	xmlNodePtr waypoint = NULL;
	char buff[256];
	if (flag == 0){
		waypoint = xmlNewNode(ns, BAD_CAST "wpt");
	} else if (flag == 1){
		waypoint = xmlNewNode(ns, BAD_CAST "rtept");
	} else if (flag == 2){
		waypoint = xmlNewNode(ns, BAD_CAST "trkpt");
	}
	if(strlen(wpt->name) > 0){
		xmlNewChild(waypoint, ns, BAD_CAST "name", BAD_CAST wpt->name);
	}
	sprintf(buff, "%lf", wpt->latitude);
	xmlNewProp(waypoint, BAD_CAST "lat", BAD_CAST buff);
	sprintf(buff, "%lf", wpt->longitude);
	xmlNewProp(waypoint, BAD_CAST "lon", BAD_CAST buff);
	
	ListIterator dataIter = createIterator(wpt->otherData);

	void *data;
	while ((data = nextElement(&dataIter))){
		GPXData *otherData = (GPXData*)data;
		xmlNewChild(waypoint, ns, BAD_CAST otherData->name, BAD_CAST otherData->value);
	}
	return waypoint;
}

xmlNodePtr RouteToXml(Route *rte, xmlNsPtr ns){
	xmlNodePtr route = NULL;

	route = xmlNewNode(ns, BAD_CAST "rte");
	if(strlen(rte->name) > 0){
		xmlNewChild(route, ns, BAD_CAST "name", BAD_CAST rte->name);
	}


	ListIterator wptIter = createIterator(rte->waypoints);
	ListIterator dataIter = createIterator(rte->otherData);

	void *data;

	while ((data = nextElement(&dataIter))){
		GPXData *otherData = (GPXData*)data;
		xmlNewChild(route, ns, BAD_CAST otherData->name, BAD_CAST otherData->value);
	}

	while ((data = nextElement(&wptIter))){
		Waypoint *wpt = (Waypoint*)data;
		xmlNode *waypoint = WaypointToXml(wpt, ns, 1);
		xmlAddChild(route, waypoint);
	}

	return route;

}

xmlNodePtr TrackSegmentToXml(TrackSegment *trkseg, xmlNsPtr ns){
	xmlNodePtr segment = xmlNewNode(ns, BAD_CAST "trkseg");
	ListIterator wptIter = createIterator(trkseg->waypoints);
	
	void *data;
	while((data = nextElement(&wptIter))){
		Waypoint *wpt = (Waypoint*)data;
		xmlNodePtr waypoint = WaypointToXml(wpt, ns, 2);
		xmlAddChild(segment, waypoint);
	}
	return segment;
}

xmlNodePtr TrackToXml(Track *trk, xmlNsPtr ns){
	xmlNodePtr track = xmlNewNode(ns, BAD_CAST "trk");
	if (strlen(trk->name) > 0){
		xmlNewChild(track, ns, BAD_CAST "name", BAD_CAST trk->name);
	}

	ListIterator segIter = createIterator(trk->segments);
	ListIterator dataIter = createIterator(trk->otherData);

	void *data;
	while((data = nextElement(&dataIter))){
		GPXData *otherData = (GPXData*)data;
		xmlNewChild(track, NULL, BAD_CAST otherData->name, BAD_CAST otherData->value);
	}

	while((data = nextElement(&segIter))){
		TrackSegment *trkseg = (TrackSegment*)data;
		xmlNodePtr segment = TrackSegmentToXml(trkseg, ns);
		xmlAddChild(track, segment);
	}

	return track;
}

xmlDocPtr GPXtoXml(GPXdoc * doc){
	xmlDocPtr newDoc = NULL;
	xmlNodePtr root = NULL;
	xmlNodePtr wpts;
	xmlNodePtr trks;
	xmlNodePtr rtes;
	char buff[256];

	newDoc = xmlNewDoc(BAD_CAST "1.0");
	root = xmlNewNode(NULL, BAD_CAST "gpx");
	sprintf(buff, "%0.1lf", doc->version);
	xmlNewProp(root, BAD_CAST "creator", BAD_CAST doc->creator);
	xmlNewProp(root, BAD_CAST "version", BAD_CAST buff);
	xmlDocSetRootElement(newDoc, root);
	xmlNsPtr ns = xmlNewNs(root, BAD_CAST doc->namespace, NULL);
	xmlSetNs(root, ns);
	

	ListIterator wptIter = createIterator(doc->waypoints);
	ListIterator rteIter = createIterator(doc->routes);
	ListIterator trkIter = createIterator(doc->tracks);
	
	void *data;
	while ((data = nextElement(&wptIter))){
		Waypoint *wpt = (Waypoint*)data;
		wpts = WaypointToXml(wpt, ns, 0);
		//xmlSetNs(wpts, ns);
		xmlAddChild(root, wpts);
	}
	while((data = nextElement(&rteIter))){
		Route *rte = (Route*)data;
		rtes = RouteToXml(rte, ns);
		//xmlSetNs(rtes, ns);
		xmlAddChild(root, rtes);
	}
	while((data = nextElement(&trkIter))){
		Track *trk = (Track*)data;
		trks = TrackToXml(trk, ns);
		//xmlSetNs(trks, ns);
		xmlAddChild(root, trks);
	}
	return newDoc;
}

bool validateGPXDoc(GPXdoc* doc, char* gpxSchemaFile){
	if (doc == NULL) {
        return false;
    }

	xmlSchemaPtr schema = NULL;
	xmlSchemaParserCtxtPtr ctxt;
	int ret;

	xmlLineNumbersDefault(1);

	ctxt = xmlSchemaNewParserCtxt(gpxSchemaFile);

	xmlSchemaSetParserErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
	schema = xmlSchemaParse(ctxt);
	xmlSchemaFreeParserCtxt(ctxt);

	LIBXML_TEST_VERSION

	xmlDocPtr mydoc = GPXtoXml(doc);

	
    if (mydoc == NULL) {
        return NULL;
    } else {
		xmlSchemaValidCtxtPtr ctxt;
 
		ctxt = xmlSchemaNewValidCtxt(schema);
		xmlSchemaSetValidErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
		ret = xmlSchemaValidateDoc(ctxt, mydoc);

		if(ret == 0){
			xmlSchemaFreeValidCtxt(ctxt);
			xmlFreeDoc(mydoc);
		} else {
			xmlSchemaFreeValidCtxt(ctxt);
			xmlFreeDoc(mydoc);
			return false;
		}
	}	

	if(schema != NULL){
		xmlSchemaFree(schema);
	}

	xmlSchemaCleanupTypes();
	xmlCleanupParser();
	xmlMemoryDump();


	return validateGPXHeader(doc);
}

bool writeGPXdoc(GPXdoc *doc, char *filename){
	if(!doc || !filename){
		return false;
	}

	char *dot = strrchr(filename, '.');
	if(strcmp(dot, ".gpx") != 0){
		free(dot);
		return false;
	}

	FILE *fp = fopen(filename, "w+");
	if(!fp){
		free(dot);
		return false;
	}
	xmlDocPtr xmlDoc = GPXtoXml(doc);
	int result = xmlSaveFormatFileEnc(filename, xmlDoc, "UTF-8", 1);
	xmlFreeDoc(xmlDoc);
	fclose(fp);
	xmlCleanupParser();

	if(result < 0){
		return false;
	}
	return true;
}

float round10(float len){
	int quotient = len/10;

	float a = quotient * 10;
	float b = quotient * 10 + 10;
 
    return (len - a >= b - len)? b : a;
}

float waypointDistance(Waypoint *first, Waypoint *second){
	int R = 6371; //Radius of the Earth
	double lat1 = first->latitude;
	double lat2 = second->latitude;
	double lon1 = first->longitude;
	double lon2 = second->longitude;

	double lat1tmp = lat1 * (M_PI / 180);
	double lat2tmp = lat2 * (M_PI /180);
	double latDiff = (lat2tmp - lat1tmp);
	double lonDiff = (lon2 - lon1) * (M_PI/180);


	double a = sin(latDiff/2) * sin(latDiff/2) + cos(lat1tmp) * cos(lat2tmp) * sin(lonDiff/2) * sin(lonDiff/2);
	double c = 2 * atan2(sqrt(a), sqrt(1-a));
    float distance = R * c;

	return distance * 1000;
}

float getRouteLen(const Route *rte){
	if (!rte || rte->waypoints->length < 2){
		return 0;
	}

	void *data;
	Waypoint *firstWpt;
	ListIterator wptIter = createIterator(rte->waypoints);
	data = nextElement(&wptIter);
	firstWpt = (Waypoint*)data;

	float distance = 0.0;

	while ((data = nextElement(&wptIter))){
		Waypoint *wpt = (Waypoint*)data;
		//printf("FIRST:%lf\n", waypointDistance(firstWpt, wpt));
		distance = distance +  waypointDistance(firstWpt, wpt);
		firstWpt = wpt;
	}
	return distance;
}

float getTrackLen(const Track *trk){
	if (!trk){
		return 0;
	}
	int i = 0;
	float distance = 0.0;
	ListIterator segIter = createIterator(trk->segments);
	Waypoint *wpt;
	void *data, *data2;
	while ((data = nextElement(&segIter))){
		TrackSegment *seg = (TrackSegment*)data;
		ListIterator wptIter = createIterator(seg->waypoints);
		data2 = nextElement(&wptIter);
		Waypoint *firstWpt = (Waypoint*)data2;
		if (i > 0){
			distance = distance + waypointDistance(wpt, firstWpt);	
		}
		while ((data2 = nextElement(&wptIter))){
			wpt = (Waypoint*)data2;
			distance = distance + waypointDistance(firstWpt, wpt);
			firstWpt = wpt;
		}
		i++;
	}
	return distance;
}

int numRoutesWithLength(const GPXdoc *doc, float len, float delta){
	if (!doc || len < 0 || delta < 0){
		return 0;
	}
	ListIterator rteIter = createIterator(doc->routes);
	float distance = 0;
	int count = 0;
	
	void *data;
	while ((data = nextElement(&rteIter))){
		Route *rte = (Route*)data;
		distance = getRouteLen(rte);
		if(fabs(distance - len) <= delta){
			count++;
		}
	}
	return count;
}

int numTracksWithLength(const GPXdoc *doc, float len, float delta){
	if (!doc || len < 0 || delta < 0){
		return 0;
	}
	ListIterator trkIter = createIterator(doc->tracks);
	float distance = 0;
	int count = 0;
	
	void *data;
	while ((data = nextElement(&trkIter))){
		Track *trk = (Track*)data;
		distance = getTrackLen(trk);
		if(fabs(distance - len) <= delta){
			count++;
		}
	}
	return count;
}

int countListOfWaypoints(List *waypoints){
	ListIterator wptIter = createIterator(waypoints);
	int count = 0;

	void *data;
	while ((data = nextElement(&wptIter))){
		count++;
	}
	return count;
}

bool isLoopRoute(const Route *rte, float delta){
	if(!rte || delta < 0){
		return false;
	}

	if(countListOfWaypoints(rte->waypoints) < 4){
		return false;
	}
	ListIterator wptIter = createIterator(rte->waypoints);
	void *data, *data2;
	data = nextElement(&wptIter);
	Waypoint *firstPoint = (Waypoint*)data;
	Waypoint *lastPoint = NULL;

	while ((data2 = nextElement(&wptIter))){
		lastPoint = (Waypoint*)data2;
	}

	if(waypointDistance(firstPoint,lastPoint) < delta){
		return true;
	}

	return false;
}

Waypoint *getLastTrackPoint(Track *trk){
	ListIterator segIter = createIterator(trk->segments);
	void *data, *data2;
	data = nextElement(&segIter);
	TrackSegment *trkseg = (TrackSegment*)data;
	ListIterator wptIter = createIterator(trkseg->waypoints);
	data2 = nextElement(&wptIter);
	Waypoint *firstPoint = (Waypoint*)data2;
	Waypoint *lastPoint = firstPoint;

	while((data2 = nextElement(&wptIter))){
		lastPoint = (Waypoint*)data2;
	}

	while ((data = nextElement(&segIter))){
		trkseg = (TrackSegment*)data;
		while((data2 = nextElement(&wptIter))){
			lastPoint = (Waypoint*)data2;
		}
	}
	return lastPoint;
}

Waypoint *getFirstTrackPoint(Track *trk){
	ListIterator segIter = createIterator(trk->segments);
	
	TrackSegment *trkseg = (TrackSegment*)segIter.current->data;
	
	ListIterator wptIter = createIterator(trkseg->waypoints);
	
	Waypoint *firstPoint = (Waypoint*)wptIter.current->data;
	return firstPoint;
}

bool isLoopTrack(const Track *trk, float delta){
	if(!trk || delta < 0){
		return false;
	}
	int wptCount = 0;
	ListIterator segIter = createIterator(trk->segments);
	void *data;

	
	Waypoint *firstPoint = NULL;
	Waypoint *lastPoint = NULL;

	while ((data = nextElement(&segIter))){
		TrackSegment *trkseg = (TrackSegment*)data;
		wptCount = wptCount + countListOfWaypoints(trkseg->waypoints);
	}

	if(wptCount < 4){
		return false;
	}

	firstPoint = getFirstTrackPoint((Track*)trk);
	lastPoint = getLastTrackPoint((Track*)trk);
	
	if(waypointDistance(firstPoint, lastPoint) < delta){
		return true;
	}
	return false;
}

Waypoint *getLastRoutePoint(Route *rte){
	ListIterator wptIter = createIterator(rte->waypoints);
	void *data;
	Waypoint *lastPoint = NULL;
	while((data = nextElement(&wptIter))){
		lastPoint = (Waypoint*)data;
	}
	return lastPoint;
}

Waypoint *getFirstRoutePoint(Route *rte){
	ListIterator wptIter = createIterator(rte->waypoints);
	void *data;
	Waypoint *firstPoint = NULL;
	data = nextElement(&wptIter);
	if(data){
		firstPoint = (Waypoint*)data;
	}
	return firstPoint;
}

List *getRoutesBetween(const GPXdoc*doc, float sourceLat, float sourceLong, float destLat, float destLong, float delta){
	if (!doc){
		return NULL;
	}
	int count = 0;
	List *tmpList = initializeList(routeToString, dummyDelete, compareRoutes);
	ListIterator rteIter = createIterator(doc->routes);
	void *data;
	Waypoint *source = malloc(sizeof(Waypoint));
	source->latitude = sourceLat;
	source->longitude = sourceLong;
	Waypoint *dest = malloc(sizeof(Waypoint));
	dest->latitude = destLat;
	dest->longitude = destLong;


	while ((data = nextElement(&rteIter))){
		Route *rte = (Route*)data;
		Waypoint *firstPoint = getFirstRoutePoint(rte);
		Waypoint *lastPoint = getLastRoutePoint(rte);
		if (firstPoint && lastPoint && waypointDistance(firstPoint, source) < delta && waypointDistance(lastPoint, dest) < delta){
			insertBack(tmpList, rte);
			count++;
		}	
	}
	free(source);
	free(dest);


	if(count == 0){
		free(tmpList);
		return NULL;
	} else {
		return tmpList;
	}
}

List *getTracksBetween(const GPXdoc*doc, float sourceLat, float sourceLong, float destLat, float destLong, float delta){
	if(!doc){
		return NULL;
	}

	List *tmpList = initializeList(routeToString, dummyDelete, compareTracks);
	ListIterator trkIter = createIterator(doc->tracks);
	Waypoint *source = malloc(sizeof(Waypoint));
	source->latitude = sourceLat;
	source->longitude = sourceLong;
	Waypoint *dest = malloc(sizeof(Waypoint));
	dest->latitude = destLat;
	dest->longitude = destLong;
	int count = 0;

	void *data;
	while((data = nextElement(&trkIter))){
		Track *trk = (Track*)data;
		Waypoint *first = getFirstTrackPoint(trk);
		Waypoint *last = getLastTrackPoint(trk);
		if(waypointDistance(first, source) < delta && waypointDistance(last, dest) < delta){
			insertBack(tmpList, trk);
			count++;
		}
	}

	free(source);
	free(dest);

	if(count == 0){
		free(tmpList);
		return NULL;
	} else {
		return tmpList;
	}

}

char *findTracksBetweenJSON(char *gpxFile, char *xsdFile, float sourceLat, float sourceLong, float destLat, float destLong){
	GPXdoc *mydoc = createValidGPXdoc(gpxFile, xsdFile);
	List *tracksBetween = getTracksBetween(mydoc, sourceLat, sourceLong, destLat, destLong, 10);
	return trackListToJSON(tracksBetween);
}

char *findRoutesBetweenJSON(char *gpxFile, char *xsdFile, float sourceLat, float sourceLong, float destLat, float destLong){
	GPXdoc *mydoc = createValidGPXdoc(gpxFile, xsdFile);
	List *routesBetween = getRoutesBetween(mydoc, sourceLat, sourceLong, destLat, destLong, 10);
	return routeListToJSON(routesBetween);
}

int checkNameField(char *name){
	if (strlen(name) > 0){
		return 1;
	} else {
		return 0;
	}
}

char *routeToJSON(const Route *rt){
	if(rt == NULL){
		return "{}";
	}
	int numWpts = countListOfWaypoints(rt->waypoints);
	bool loopStat = isLoopRoute((Route*)rt, 10);
	char *JSONstring = malloc(sizeof(char)* 128);
	int nameStat = checkNameField(rt->name);

	sprintf(JSONstring, "{\"name\":\"%s\",\"numPoints\":%d,\"len\":%0.1f,\"loop\":%s}", nameStat == 1? rt->name : "none", numWpts, round10(getRouteLen((Route*)rt)), loopStat? "true" : "false");
	return JSONstring;
}

int getTrackPointCount(const Track *trk){
	ListIterator iter = createIterator(trk->segments);
	int sum = 0;
	TrackSegment *trkseg;
	void *data, *data2;
	while ((data = nextElement(&iter)) != NULL){
		trkseg = (TrackSegment*)data;
		ListIterator iter2 = createIterator(trk->segments);
		while ((data2 = nextElement(&iter2)) != NULL){
			sum += countListOfWaypoints(trkseg->waypoints);	
		}
	}
	return sum;
}

char *trackToJSON(const Track *trk){
	if(trk == NULL){
		return "{}";
	}
	bool loopStat = isLoopTrack((Track*)trk, 10);
	char *JSONstring = malloc(sizeof(char)* 128 );
	int nameStat = checkNameField(trk->name);
	sprintf(JSONstring, "{\"name\":\"%s\",\"numPoints\":%d,\"len\":%0.1f,\"loop\":%s}", nameStat == 1? trk->name : "none", getTrackPointCount(trk), round10(getTrackLen((Track*)trk)), loopStat? "true" : "false");
	return JSONstring;
}

char *otherDataToJSON(GPXData *otherData){
	char *JSONstring = malloc(sizeof(char) * 128);
	sprintf(JSONstring, "{\"name\":\"%s\",\"value\":\"%s\"}", otherData->name, otherData->value);
	return JSONstring;
}

char *otherDataListToJSON(List *otherData){
	if (!otherData){
		return "[]";
	}
	ListIterator dataIter = createIterator(otherData);
	void *data;
	data = nextElement(&dataIter);
	if(!data){
		return "[]";
	}
	GPXData *gpxData = (GPXData*)data;
	char *dataJSON = otherDataToJSON(gpxData);
	char *JSONstring = malloc(sizeof(char)*strlen(dataJSON) + 4);
	sprintf(JSONstring, "[%s,", dataJSON);
	free(dataJSON);
	while ((data = nextElement(&dataIter))){
		gpxData = (GPXData*)data;
		dataJSON = otherDataToJSON(gpxData);
		JSONstring = realloc(JSONstring, strlen(JSONstring) + strlen(dataJSON) + 4);
		if (JSONstring){
			strcat(JSONstring, dataJSON);
			strcat(JSONstring, ",");
		}
		free(dataJSON);
	}
	JSONstring[strlen(JSONstring) - 1] = ']';
	return JSONstring;
}

char *routeListToJSON(const List *routeList){
	if(!routeList){
		return "[]";
	}
	ListIterator rteIter = createIterator((List*)routeList);
	void *data;
	data = nextElement(&rteIter);
	if(!data){
		return "[]";
	}
	Route *rte = (Route*)data;
	char *routeJSON = routeToJSON(rte);
	char *JSONstring = malloc(sizeof(char)* strlen(routeJSON) + 4);
	sprintf(JSONstring, "[%s,", routeJSON);
	free(routeJSON);
	while ((data = nextElement(&rteIter))){
		rte = (Route*)data;
		routeJSON = routeToJSON(rte);
		JSONstring = realloc(JSONstring, strlen(JSONstring) + strlen(routeJSON) + 4);
		if (JSONstring){
			strcat(JSONstring, routeJSON);
			strcat(JSONstring, ",");
		}
		free(routeJSON);
	}
	JSONstring[strlen(JSONstring) - 1] = ']';
	return JSONstring;
}

char *trackListToJSON(const List *trackList){
	if(!trackList){
		return "[]";
	}
	ListIterator trkIter = createIterator((List*)trackList);
	void *data;
	data = nextElement(&trkIter);
	if(!data){
		return "[]";
	}
	Track *trk = (Track*)data;
	char *trackJSON = trackToJSON(trk);
	char *JSONstring = malloc(sizeof(char)* strlen(trackJSON) + 4);
	sprintf(JSONstring, "[%s,", trackJSON);
	free(trackJSON);
	while ((data = nextElement(&trkIter))){
		trk = (Track*)data;
		trackJSON = trackToJSON(trk);
		JSONstring = realloc(JSONstring, strlen(JSONstring) + strlen(trackJSON) + 4);
		if (JSONstring){
			strcat(JSONstring, trackJSON);
			strcat(JSONstring, ",");
		}
		free(trackJSON);
	}
	JSONstring[strlen(JSONstring) - 1] = ']';
	return JSONstring;
}

char *GPXtoJSON(const GPXdoc *doc){
	if(!doc){
		return "{}";
	}
	char *JSONstring = malloc(sizeof(char) * 128);
	sprintf(JSONstring, "{\"version\":%0.1f,\"creator\":\"%s\",\"numWaypoints\":%d,\"numRoutes\":%d,\"numTracks\":%d}", doc->version, doc->creator, getNumWaypoints((GPXdoc*)doc), getNumRoutes((GPXdoc*)doc), getNumTracks((GPXdoc*)doc));
	return JSONstring;
}


void addWaypoint(Route *rte, Waypoint *wpt){
	if (!rte || !wpt){
		return;
	}
	insertBack(rte->waypoints, (void*)wpt);
}

void addRoute(GPXdoc *doc, Route *rte){
	if (!doc || !rte){
		return;
	}
	insertBack(doc->routes, (void*)rte);
}

GPXdoc *JSONtoGPX(const char *gpxString){
	GPXdoc *newdoc = malloc(sizeof(GPXdoc));
	newdoc->waypoints = initializeList(&waypointToString, &deleteWaypoint, &compareWaypoints);
    newdoc->routes = initializeList(&routeToString, &deleteRoute, &compareRoutes);
    newdoc->tracks = initializeList(&trackToString, &deleteTrack, &compareTracks);
	strcpy(newdoc->namespace, "http://www.topografix.com/GPX/1/1");
	newdoc->creator = malloc(sizeof(char));
	strcpy(newdoc->creator, "\0") ;

	return newdoc;
}

Waypoint *JSONtoWaypoint(const char *gpxString){
	Waypoint *newdoc = malloc(sizeof(Waypoint));
	newdoc->otherData = initializeList(gpxDataToString, deleteGpxData, compareGpxData);
	newdoc->latitude = 1;
	newdoc->longitude = 1;
	return newdoc;
}

Route *JSONtoRoute(const char *gpxString){
	Route *newdoc = malloc(sizeof(Route));
	newdoc->name = malloc(sizeof(char));
	strcpy(newdoc->name, "\0") ;
	newdoc->waypoints = initializeList(&waypointToString, &deleteWaypoint, &compareWaypoints);
	newdoc->otherData = initializeList(gpxDataToString, deleteGpxData, compareGpxData);
	return newdoc;
}

int getNumWaypoints(const GPXdoc *mydoc){
	if(mydoc == NULL){
		return 0;
	}
	int i = 0;
	ListIterator iter = createIterator(mydoc->waypoints);
	while (nextElement(&iter)){
		i++;
	}
	return i;
}

int getNumRoutes(const GPXdoc *mydoc){
	if(mydoc == NULL){
		return 0;
	}
	int i = 0;
	ListIterator iter = createIterator(mydoc->routes);
	while (nextElement(&iter)){
		i++;
	}
	return i;
}

int getNumTracks(const GPXdoc *mydoc){
	if(mydoc == NULL){
		return 0;
	}
	int i = 0;
	ListIterator iter = createIterator(mydoc->tracks);
	while (nextElement(&iter)){
		i++;
	}
	return i;
}

int getNumSegments(const GPXdoc *mydoc){
	if(mydoc == NULL){
		return 0;
	}
	int i = 0;
	ListIterator iter = createIterator(mydoc->tracks);
	ListIterator segIter;
	Track *trk;
	void *data;
	while ((data = nextElement(&iter)) != NULL){
		trk = (Track*)data;
		segIter = createIterator(trk->segments);
		while (nextElement(&segIter)){
			i++;
		}
	}
	return i;
}

int countGPXData(List *gpxDataList){
	int i = 0;
	ListIterator iter = createIterator(gpxDataList);
	while (nextElement(&iter)){
		i++;
	}
	return i;
}

int getNumGPXData(const GPXdoc *mydoc){
	int sum = 0;
	if (mydoc == NULL){
		return 0;
	}

//count waypoints
	ListIterator iter = createIterator(mydoc->waypoints);
	ListIterator iter2, iter3;
	void *data, *data2, *data3;
	Waypoint *wpt;
	while ((data = nextElement(&iter)) != NULL){
		wpt = (Waypoint*)data;
		sum += checkNameField(wpt->name);
		sum += countGPXData(wpt->otherData);
	}


//count routes & route waypoints
	iter = createIterator(mydoc->routes);
	Route *rte;
	while ((data = nextElement(&iter)) != NULL){
		rte = (Route*)data;
		iter2 = createIterator(rte->waypoints);
		while ((data2 = nextElement(&iter2)) != NULL){
			wpt = (Waypoint*)data2;
			sum += countGPXData(wpt->otherData);
			sum += checkNameField(wpt->name);
		}
		sum += checkNameField(rte->name);
		sum += countGPXData(rte->otherData);
	}


//count tracks/tracksegments/track waypoints
	iter = createIterator(mydoc->tracks);
	Track *trk;
	TrackSegment *trkseg;
	while ((data = nextElement(&iter)) != NULL){
		trk = (Track*)data;
		iter2 = createIterator(trk->segments);
		while ((data2 = nextElement(&iter2)) != NULL){
			trkseg = (TrackSegment*)data2;
			iter3 = createIterator(trkseg->waypoints);
			while ((data3 = nextElement(&iter3)) != NULL){
				wpt = (Waypoint*)data3;
				sum += checkNameField(wpt->name);
				sum += countGPXData(wpt->otherData);
			}
		}
		sum += checkNameField(trk->name);
		sum += countGPXData(trk->otherData);
	}


	return sum;
}


Waypoint* getWaypoint(const GPXdoc *mydoc, char *name){

	if (mydoc == NULL || name == NULL){
		return NULL;
	}

	ListIterator iter = createIterator(mydoc->waypoints);
	void *data;
	Waypoint *wpt;

	while ((data = nextElement(&iter)) != NULL){
		wpt = (Waypoint*)data;
		if (strcmp(wpt->name, name) == 0){
			return wpt;
		}
	}
	return NULL;
}

Track* getTrack(const GPXdoc *mydoc, char *name){

	if (mydoc == NULL || name == NULL){
		return NULL;
	}


	ListIterator iter = createIterator(mydoc->tracks);
	void *data;
	Track *trk;

	while ((data = nextElement(&iter)) != NULL){
		trk = (Track*)data;
		if (strcmp(trk->name, name) == 0){
			return trk;
		}
	}
	return NULL;
}

Route* getRoute(const GPXdoc *mydoc, char *name){

	if (mydoc == NULL || name == NULL){
		return NULL;
	}


	ListIterator iter = createIterator(mydoc->routes);
	void *data;
	Route *rte;

	while ((data = nextElement(&iter)) != NULL){
		rte = (Route*)data;
		if (strcmp(rte->name, name) == 0){
			return rte;
		}
	}
	return NULL;
}

char *getGPXinfo(char *gpxFile, char *xsdFile){
	GPXdoc *mydoc = createValidGPXdoc(gpxFile, xsdFile);
	return GPXtoJSON(mydoc);
}

char *getRouteListJSON(char *gpxFile, char *xsdFile){
	GPXdoc *mydoc = createValidGPXdoc(gpxFile, xsdFile);
	return routeListToJSON(mydoc->routes);
}

char *getTrackListJSON(char *gpxFile, char *xsdFile){
	GPXdoc *mydoc = createValidGPXdoc(gpxFile, xsdFile);
	return trackListToJSON(mydoc->tracks);
}

int isValidGPXFile(char *gpxFile, char *xsdFile){
	GPXdoc *mydoc = createValidGPXdoc(gpxFile, xsdFile);
	if (mydoc){
		return 1;
	} else {
		return 0;
	}
}

char *getTrackOtherData(char *gpxFile, char *xsdFile, int index){
	GPXdoc *mydoc = createValidGPXdoc(gpxFile, xsdFile);
	ListIterator iter = createIterator(mydoc->tracks);
	void *data = nextElement(&iter);
	int i = 1;
	while (i != index){
		data = nextElement(&iter);
		i++;
	}
	Track *trk = (Track*)data;
	return otherDataListToJSON(trk->otherData);
}

char *getRouteOtherData(char *gpxFile, char *xsdFile, int index){
	GPXdoc *mydoc = createValidGPXdoc(gpxFile, xsdFile);
	ListIterator iter = createIterator(mydoc->routes);
	void *data = nextElement(&iter);
	int i = 1;
	while (i != index){
		data = nextElement(&iter);
		i++;
	}
	Route *rte = (Route*)data;
	return otherDataListToJSON(rte->otherData);
}


bool changeTrackName(char *gpxFile, char *xsdFile, int index, char *newName){
	GPXdoc *mydoc = createValidGPXdoc(gpxFile, xsdFile);
	ListIterator iter = createIterator(mydoc->tracks);
	void *data = nextElement(&iter);
	int i = 1;
	while (i != index){
		data = nextElement(&iter);
		i++;
	}
	Track *trk = (Track*)data;
	trk->name = newName;
	if (!(validateGPXDoc(mydoc, xsdFile))){
		return false;
	}
	return writeGPXdoc(mydoc, gpxFile);
}

bool changeRouteName(char *gpxFile, char *xsdFile, int index, char *newName){
	GPXdoc *mydoc = createValidGPXdoc(gpxFile, xsdFile);
	ListIterator iter = createIterator(mydoc->routes);
	void *data = nextElement(&iter);
	int i = 1;
	while (i != index){
		data = nextElement(&iter);
		i++;
	}
	Route *rte = (Route*)data;
	rte->name = newName;
	if (!(validateGPXDoc(mydoc, xsdFile))){
		return false;
	}
	return writeGPXdoc(mydoc, gpxFile);
}

bool addWaypointToRoute(char *gpxFile, char *xsdFile, float lat, float lon){
	GPXdoc *mydoc = createValidGPXdoc(gpxFile, xsdFile);
	ListIterator iter = createIterator(mydoc->routes);
	void *data = nextElement(&iter);
	Route *rte = (Route*)data;

	while((data = nextElement(&iter))){
		rte = (Route*)data;
	}


	Waypoint *wpt = malloc(sizeof(Waypoint));
	wpt->otherData = initializeList(gpxDataToString, deleteGpxData, compareGpxData);
	wpt->name = malloc(sizeof(char) + 2);
	strcpy(wpt->name, "\0");
	wpt->latitude = lat;
	wpt->longitude = lon;
	insertBack(rte->waypoints, wpt);
	if (!(validateGPXDoc(mydoc, xsdFile))){
		return false;
	}
	return writeGPXdoc(mydoc, gpxFile);
}

bool addRouteToGPX(char *gpxFile, char *xsdFile, char *name){
	GPXdoc *mydoc = createValidGPXdoc(gpxFile, xsdFile);
	if(!mydoc){
		return false;
	}
	Route *rte = malloc(sizeof(Route));
	rte->waypoints = initializeList(&waypointToString, &deleteWaypoint, &compareWaypoints);
	rte->otherData = initializeList(gpxDataToString, deleteGpxData, compareGpxData);
	if(name){
		rte->name = malloc(sizeof(char) * strlen(name));
		strcpy(rte->name, name);
	} else {
		rte->name = malloc(sizeof(char) * 1);
		strcpy(rte->name, "\0");
	}
	addRoute(mydoc, rte);
	if (!(validateGPXDoc(mydoc, xsdFile))){
		return false;
	}
	return writeGPXdoc(mydoc, gpxFile);
}

bool createNewGPX(char *file, char *xsdFile, double version, char *creator){
	GPXdoc *mydoc = malloc(sizeof(GPXdoc));
	mydoc->waypoints = initializeList(&waypointToString, &deleteWaypoint, &compareWaypoints);
    mydoc->routes = initializeList(&routeToString, &deleteRoute, &compareRoutes);
    mydoc->tracks = initializeList(&trackToString, &deleteTrack, &compareTracks);
	mydoc->version = version;
	strcpy(mydoc->namespace, "http://www.topografix.com/GPX/1/1");
	mydoc->creator = malloc(sizeof(char)*strlen(creator) + 1);
	strcpy(mydoc->creator, creator);
	if (!(validateGPXDoc(mydoc, xsdFile))){
		return false;
	}
	return writeGPXdoc(mydoc, file);
}