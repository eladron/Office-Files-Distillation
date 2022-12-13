#ifndef TAGS_H
#define TAGS_H

#define PARAGRAPH "w:p"
#define RUN "w:r"
#define TEXT "w:t"
#define PARAGRAPH_PROPERTY "w:pPr"
#define PARAGRAPH_STYLE "w:pStyle"
#define VALUE "w:val"
#define NUM_PROPERTY "w:numPr"
#define LEVEL "w:ilvl"
#define LIST_PARAGRAPH "ListParagraph"
#define NOT_IN_LIST -1
#define HYPERLINK "w:hyperlink"
#define TABLE "w:tbl"
#define TABLE_ROW "w:tr"
#define TABLE_COLUMN "w:tc"
#define NAME "w:name"

#define DRAWING "w:drawing"
#define INLINE "wp:inline"
#define ANCHOR "wp:anchor"
#define GRAPHIC "a:graphic"
#define GRAPHIC_DATA "a:graphicData"
#define PIC "pic:pic"
#define PIC_FILL "pic:blipFill"
#define BLIP "a:blip"
#define EMBED "r:embed"
#define IMAGE_PATH (std::vector<const char*>({GRAPHIC, GRAPHIC_DATA, PIC, PIC_FILL, BLIP}))

#endif
