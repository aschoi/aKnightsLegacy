#include "knightsLegacy/world/public/TileCatalog.h"

static const std::vector<TileDef> gTileCatalog = {
    {0, 1.0, 6.0, false, false},   // 0 floor, open, light colored
    {0, 1.0, 7.0, false, false},   // 1 floor, bot border

    {0, 1, 3, true, false},    // 2 wall, fully
    {0, 1, 4, true, false},    // 3 wall, partial

    {0, 3, 18, false, false},   // 4 open door, TL
    {0, 4, 18, false, false},   // 5 open door, TR
    {0, 3, 19, false, false},  // 6 open door, ML
    {0, 4, 19, false, false},  // 7 open door, MR
    {0, 3, 20, false, false},  // 8 open door, BL
    {0, 4, 20, false, false},  // 9 open door, BR

    {1, 6, 4, false, false},    // 10 barrel top
    {1, 6, 5, false, false},    // 11 barrel bot
    {1, 10, 2, false, false},   // 12 chest with gold TL
    {1, 11, 2, false, false},   // 13 chest with gold TR
    {1, 10, 3, false, false},   // 14 chest with gold BL
    {1, 11, 3, false, false},   // 15 chest with gold BR

    // SCREEN BACKGROUND
    {2, 14, 2, false, false},  // 16 start menu base layer TL corner
    {2, 15, 2, false, false},  // 17 start menu base layer TM side
    {2, 19, 2, false, false},  // 18 start menu base layer TR corner
    {2, 14, 3, false, false},  // 19 start menu base layer LM side
    {2, 15, 3, false, false},  // 20 start menu base layer MM middle
    {2, 19, 3, false, false},  // 21 start menu base layer RM side
    {2, 14, 5, false, false},  // 22 start menu base layer BL corner
    {2, 15, 5, false, false},  // 23 start menu base layer BM side
    {2, 19, 5, false, false},  // 24 start menu base layer BR corner

    // BACKGROUND FOR TEXT
    {2, 20, 2, false, false},  // 25 start menu text background TL corner
    {2, 21, 2, false, false},  // 26 start menu text background TM side
    {2, 23, 2, false, false},  // 27 start menu text background TR corner
    {2, 20, 3, false, false},  // 28 start menu text background BL corner
    {2, 21, 3, false, false},  // 29 start menu text background BM side
    {2, 23, 3, false, false},  // 30 start menu text background BR corner

    // CURRENT SELECTION
    {2, 24, 2, false, false},  // 31 start menu selection TL corner
    {2, 25, 2, false, false},  // 32 start menu selection TR corner
    {2, 24, 3, false, false},  // 33 start menu selection BL corner
    {2, 25, 3, false, false},  // 34 start menu selection BR corner

    // BANNER 1
    {2, 18, 6, false, false},  // 35 start menu title banner TL corner
    {2, 19, 6, false, false},  // 36 start menu title banner TM side
    {2, 21, 6, false, false},  // 37 start menu title banner TR corner
    {2, 18, 7, false, false},  // 38 start menu title banner BL corner
    {2, 19, 7, false, false},  // 39 start menu title banner BM side
    {2, 21, 7, false, false},  // 40 start menu title banner BR corner

    // BUTTON BACKGROUND
    {4, 0, 0, false, false},  // 41 start button background T1
    {4, 1, 0, false, false},  // 42 start button background T2 
    {4, 2, 0, false, false},  // 43 start button background T3 
    {4, 4, 0, false, false},  // 44 start button background T4 
    {4, 5, 0, false, false},  // 45 start button background T5 

    {4, 0, 1, false, false},  // 46 start button background TM1
    {4, 1, 1, false, false},  // 47 start button background TM2 
    {4, 5, 1, false, false},  // 48 start button background TM3
    {4, 0, 2, false, false},  // 49 start button background MM1
    {4, 1, 2, false, false},  // 50 start button background MM2 
    {4, 5, 2, false, false},  // 51 start button background MM3
    {4, 0, 4, false, false},  // 52 start button background BM1
    {4, 1, 4, false, false},  // 53 start button background BM2 
    {4, 5, 4, false, false},  // 54 start button background BM3

    {4, 0, 5, false, false},  // 55 start button background T3 
    {4, 1, 5, false, false},  // 56 start button background T4 
    {4, 2, 5, false, false},  // 57 start button background T5 
    {4, 4, 5, false, false},  // 58 start button background B1
    {4, 5, 5, false, false},  // 59 start button background B2 
    
    // BANNER
    {3, 5, 0, false, false},  // 60 start button background T1 
    {3, 6, 0, false, false},  // 61 start button background T2 
    {3, 7, 0, false, false},  // 62 start button background T3
    {3, 5, 1, false, false},  // 63 start button background M1
    {3, 6, 1, false, false},  // 64 start button background M2 
    {3, 7, 1, false, false},  // 65 start button background M3 
    {3, 5, 2, false, false},  // 66 start button background B1 
    {3, 6, 2, false, false},  // 67 start button background B2 
    {3, 7, 2, false, false},  // 68 start button background B3

    // BANNER HORROR
    {5, 8, 8, false, false},  // 69 start button background T1 
    {5, 9, 8, false, false},  // 70 start button background T2 
    {5, 11, 0, false, false}, // 71 start button background T3
    {5, 10, 8, false, false}, // 72 start button background T4
    {5, 11, 8, false, false}, // 73 start button background T5

    {5, 8, 9, false, false},  // 74 start button background M3 
    {5, 9, 9, false, false},  // 75 start button background B1 
    {5, 11, 9, false, false}, // 76 start button background B2 

    {5, 8, 12, false, false},  // 77 start button background B3
    {5, 9, 12, false, false},  // 78 start button background M3 
    {5, 11, 12, false, false }, // 79 start button background B1 

    // OPEN DOOR
    { 0, 3, 21, true, false },   // 80 open door, TL
    { 0, 4, 21, true, false },   // 81 open door, TR
    { 0, 3, 22, false, false },  // 82 open door, BL
    { 0, 4, 22, false, false },  // 83 open door, BR

    // GATED DOOR
    { 0, 5, 21, true, false },   // 84 open door, TL
    { 0, 6, 21, true, false },   // 85 open door, TR
    { 0, 5, 22, true, false },   // 86 open door, BL
    { 0, 6, 22, true, false },   // 87 open door, BR

    // small stairs Left sided enterence 
    { 0, 5, 11, false, false },   // 88 open door, TL
    { 0, 6, 11, false, false },   // 89 open door, TR
    // PLATFORM
    { 0, 0, 6, true, false },   // 90 left edge
    { 0, 0, 5, true, false },   // 91 TL corner 
    { 0, 1, 5, true, false },   // 92 top edge
    { 0, 2, 5, true, false },   // 93 TR corner
    { 0, 2, 6, true, false },   // 94 right edge
    { 0, 2, 7, true, false },   // 95 BR corner
    { 0, 2, 8, true, false },   // 96 BR wall
    { 0, 1, 7, true, false },   // 97 bot edge
    { 0, 1, 8, true, false },   // 98 bot wall
    { 0, 0, 7, true, false },   // 99 BL corner
    { 0, 0, 8, true, false },   // 100 BL wall

    // STAIRS left sided entrance
    { 0, 6, 7, false, false },   // 101 TL 
    { 0, 7, 7, false, false },   // 102 TR
    { 0, 6, 8, false, false },   // 103 BL 
    { 0, 7, 8, false, false },   // 104 BR

    // INTERIOR WALL DIVIDER
    { 0, 0, 1, true, false },   // 105 edge top wall L
    { 0, 0, 2, true, false },   // 106 corner of top wall L
    { 0, 0, 3, true, false },   // 107 edge mid wall L
    { 0, 0, 4, true, false },   // 108 corner of bot wall L

    { 0, 1, 1, true, false },   // 109 edge top wall M  (Darkness)
    { 0, 1, 2, true, false },   // 110 corner of top wall M
    { 0, 1, 3, true, false },   // 111 edge mid wall M
    { 0, 1, 4, true, false },   // 112 corner of bot wall M

    { 0, 2, 1, true, false },   // 113 edge of top wall R
    { 0, 2, 2, true, false },   // 114 corner of top wall R
    { 0, 2, 3, true, false },   // 115 edge of mid wall R
    { 0, 2, 4, true, false },   // 116 corner of bot wall R

    { 0, 8, 0, true, false },   // 117 L rounding to boundary
    { 0, 8, 1, true, false },   // 118 mid transition
    { 0, 8, 2, true, false },   // 119 bot transition

    { 0, 7, 0, true, false },   // 120 Boundary top
    { 0, 7, 1, true, false },   // 121 Boundary mid
    { 0, 7, 2, true, false },   // 122 Boundary bot
    { 0, 6, 0, true, false },   // 123 R rounding to boundary

    { 0, 0, 0, true, false },   // 124 TOP L WALL 
    { 0, 1, 0, true, false },   // 125 TOP MID WALL 
    { 0, 2, 0, true, false },   // 126 TOP R WALL 

    // OPEN WALL
    { 0, 5, 12, true, false },   // 127 1L
    { 0, 1, 15, false, false },   // 128 1M
    { 0, 6, 12, true, false },   // 129 1R

    { 0, 5, 13, false, false },   // 130 2L
    { 0, 1, 16, false, false },   // 131 2M
    { 0, 6, 13, false, false },   // 132 2R

    { 0, 5, 14, false, false },   // 133 3L
    { 0, 1, 17, false, false },   // 134 3M
    { 0, 6, 14, false, false },   // 135 3R

    { 0, 2, 14, true, false },   // 136 mid wall piece

    { 0, 5, 15, false, false },   // 137 4L
    { 0, 6, 15, false, false },   // 138 4R

    { 0, 3, 0, true, false },   // 139 corner
    { 0, 3, 2, true, false },   // 140 side
    { 0, 4, 3, true, false },   // 141 side

    { 0, 0, 15, true, false },   // 142 wall
    { 0, 0, 16, true, false },   // 143 wall
    { 0, 0, 17, true, false },   // 144 wall
    { 0, 1, 15, true, false },   // 145 wall
    { 0, 1, 16, true, false },   // 146 wall
    { 0, 1, 17, true, false },   // 147 wall
    { 0, 4, 15, true, false },   // 148 wall
    { 0, 4, 16, true, false },   // 149 wall
    { 0, 4, 17, true, false },   // 150 wall

    { 0, 11, 7, false, false },   // 151 stairs tl
    { 0, 12, 7, false, false },   // 152 stairs tr
    { 0, 11, 8, false, false },   // 153 stairs bl
    { 0, 12, 8, false, false },   // 154 stairs br

    { 0, 3, 3, true, false },   // 155 map corner BL
    { 0, 5, 3, true, false },   // 156 map corner BR
    { 0, 5, 0, true, false },   // 157 map corner TR
     
    { 0, 3, 1, true, false },   // 158 map corner 
    { 0, 3, 2, true, false },   // 159 map corner 

    { 0, 12, 15, true, false }, // 160 wall intersection
    { 0, 12, 16, true, false }, // 161 wall intersection
    { 0, 12, 17, true, false }, // 162 wall intersection
    
    { 0, 5, 2, true, false }, // 163 right wall boundary
    { 0, 0, 1, true, false }, // 164 right wall boundary

    { 6, 5, 25, true, true }, // 165 trap 1
    { 6, 6, 25, true, true }, // 166 trap 2
    { 6, 7, 25, true, true }, // 167 trap 3

    { 6, 5, 26, true, true }, // 168 trap 4
    { 6, 6, 26, true, true }, // 169 trap 5
    { 6, 7, 26, true, true }, // 170 trap 6

    { 6, 5, 29, true, true }, // 171 trap 7
    { 6, 6, 29, true, true }, // 172 trap 8
    { 6, 7, 29, true, true }, // 173 trap 9

    { 0, 9, 16, true, false }, // 174 wall boundary transition R mid
    { 0, 9, 17, true, false }, // 175 wall boundary transition R bot

    { 0, 5, 13, true, false }, // 176 wall type 2 LM
    { 0, 6, 13, true, false }, // 177 wall type 2 RM

    { 0, 2, 15, true, false }, // 178 wall type 2 wall intetegration Top
    { 0, 2, 16, true, false }, // 179 wall type 2 wall intetegration M
    { 0, 2, 17, true, false }, // 180 wall type 2 wall intetegration Bot

    { 0, 2, 18, true, false }, // 181 wall type 2 wall Top finsh
    { 0, 2, 19, true, false }, // 182 wall type 2 wall M finsh
    { 0, 2, 20, true, false }, // 183 wall type 2 wall Bot finish

    { 6, 10, 25, true, true }, // 184 trap 10
    { 6, 10, 28, true, true }, // 185 trap 11
    { 6, 10, 29, true, true }, // 186 trap 12

};

const std::vector<TileDef>& GetTileCatalog() {
    return gTileCatalog;
}