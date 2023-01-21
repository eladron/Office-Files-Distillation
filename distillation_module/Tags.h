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
#define BIDI "w:bidi"

#define MATH_PARAGRAPH ("m:oMathPara")
#define DRAWING "w:drawing"
#define INLINE "wp:inline"
#define ANCHOR "wp:anchor"
#define GRAPHIC "a:graphic"
#define GRAPHIC_DATA "a:graphicData"
#define PIC "pic:pic"
#define PIC_FILL "pic:blipFill"
#define BLIP "a:blip"
#define IMAGE_PATH (std::vector<const char*>({GRAPHIC, GRAPHIC_DATA, PIC, PIC_FILL, BLIP}))

// Run Properties
#define RUN_PROPERTY "w:rPr"
#define BOLD "w:b"
#define ITALIC "w:i"
#define UNDERLINE "w:u"
#define SIZE "w:sz"
#define JUSTIFICATION "w:jc"
#define RSTYLE "w:rStyle"
#define HIGHLIGHT "w:highlight"
#define COLOR "w:color"
#define COLOR "w:color"


//OLE
#define OBJECT "w:object"
#define SHAPE "v:shape"
#define IMAGEDATA "v:imagedata"
#define OLE "o:OLEObject"

/*
    Style Tags
*/
#define STYLE "w:style"
#define STYLE_ID "w:styleId"
#define STYLE_TYPE "w:type"
#define CHARACTER_STYLE_TYPE "character"
#define NUMBERING_STYLE_TYPE "numbering"
#define PARAGRAPH_STYLE_TYPE "paragraph"

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
#define HIGHLIGHT_VAL (std::vector<const char*>({"black","blue","cyan","darkBlue","darkCyan","darkGray","darkGreen","darkMagenta","darkRed","darkYellow","darkYellow","gray","green","magenta","none","red","white","yellow"}))
#endif
