***
Test: test_suites/interactive_directive
Creator: Levon
Maintainer: Levon
***

Summary: testing replying of interactive commands

Testing steps: 
    Creating all possible shapes via non-interactive commands
    
    dicmdCreateObjRectangle -points {(0,0);(100,100)}
    dicmdCreateObjRectangle -points {(308,201);(516,336)} 
    dicmdCreateObjRectangle -points {(510,92);(845,240)} 
    dicmdCreateObjRectangle -points {(70,147);(153,225)} 
    dicmdCreateObjRectangle -points {(225,73);(301,149)} 
    dicmdCreateObjLine -points {(149,337);(264,249)} 
    dicmdCreateObjLine -points {(267,406);(206,244)} 
    dicmdCreateObjEllipse -points {(398,11);(465,90)} 
    dicmdCreateObjEllipse -points {(479,178);(657,401)} 
    dicmdCreateObjEllipse -points {(398,11);(465,90)} 
    dicmdCreateObjEllipse -points {(479,178);(657,401)}
    dicmdCreateObjEllipse -points {(920,466);(1077,486)}
    dicmdCreateObjPolygon -points {(81,463);(82,463);(83,463);(84,463);(85,462);(87,461);(91,458);(99,454);(106,450);(110,448);(114,446);(118,444);(121,443);(125,440);(130,437);(135,434);(139,432);(141,431);(143,430);(145,429);(146,428);(147,428);(147,427);(148,427);(149,427);(149,426);(149,427);(149,428);(149,429);(149,430);(149,433);(149,434);(149,436);(149,438);(149,440);(149,443);(150,446);(152,450);(153,453);(155,457);(157,460);(159,463);(160,466);(162,469);(164,472);(165,474);(166,476);(167,477);(167,478);(168,478);(168,479);(169,479);(169,480);(169,479)} 


Expected results: 
    Shapes should be created: 8 Rect, 2 Line, 4 Ellipse, 1 Polygon
