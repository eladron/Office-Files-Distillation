#ifndef TAGS_H
#define TAGS_H

/*
    Tags Names

*/

#define PARAGRAPH "w:p"
#define RUN "w:r"
#define TEXT "w:t"
#define PARAGRAPH_PROPERTY "w:pPr"
#define PARAGRAPH_STYLE "w:pStyle"
#define NUM_PROPERTY "w:numPr"
#define LEVEL "w:ilvl"
#define LIST_PARAGRAPH "ListParagraph"
#define NOT_IN_LIST -1
#define HYPERLINK "w:hyperlink"
#define TABLE "w:tbl"
#define TABLE_ROW "w:tr"
#define TABLE_COLUMN "w:tc"
#define NAME "w:name"
#define RUN_PROPERTY "w:rPr"
#define RIGHT_TO_LEFT "w:rtl"
#define LEFT_TO_RIGHT "w:ltr"
#define BOLD "w:b"
#define ITALIC "w:i"
#define UNDERLINE "w:u"
#define SIZE "w:sz"
#define JUSTIFICATION "w:jc"
#define BIDI "w:bidi"
#define RSTYLE "w:rStyle"

#define DRAWING "w:drawing"
#define INLINE "wp:inline"
#define ANCHOR "wp:anchor"
#define GRAPHIC "a:graphic"
#define GRAPHIC_DATA "a:graphicData"
#define PIC "pic:pic"
#define PIC_FILL "pic:blipFill"
#define BLIP "a:blip"
#define IMAGE_PATH (std::vector<const char*>({GRAPHIC, GRAPHIC_DATA, PIC, PIC_FILL, BLIP}))


/*
    Attributes Naem
*/
#define VALUE "w:val"
#define RELATION_ID "r:id"
#define EMBED "r:embed"
#define HISTORY "w:history"
#define TARGET "Target"
#define TYPE "Type"
#define TARGET_MODE "TargetMode"


#define UNDERLINE_VAL (std::vector<const char*>({"dash","dashDotDotHeavy","dashDotHeavy","dashedHeavy","dashLong","dashLongHeavy","dotDash","dotDotDash","dotted","dottedHeavy","double","none","single","thick","wave","wavyDouble","wavyHeavy","words"}))

#endif
