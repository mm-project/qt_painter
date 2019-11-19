***
Test: test_suites/directive/save_load_reset_basic
Creator: Levon
Maintainer: Levon
***

Summary: basic usage of save load reset command

Testing steps: 
    Create different shapes, then saving to a file and loading back.
    
    dicmdCreateObjRectangle -points {(120,115);(250,228)} 
    dicmdCreateObjRectangle -points {(297,241);(431,351)} 
    dicmdCreateObjRectangle -points {(504,272);(645,394)} 
    dicmdCreateObjRectangle -points {(630,95);(816,237)} 
    dicmdSelectShapesByRegion -end (1211,459) -start (17,5) 
    dicmdCreateObjRectangle -points {(159,123);(281,257)} 
    dicmdCreateObjRectangle -points {(262,237);(384,381)} 
    dicmdCreateObjRectangle -points {(345,341);(492,450)} 
    dicmdCreateObjEllipse -points {(309,86);(403,160)} 
    dicmdCreateObjEllipse -points {(372,138);(479,235)} 
    dicmdCreateObjEllipse -points {(456,242);(533,309)} 
    dicmdCreateObjPolygon -points {(561,123);(711,128);(648,264);(787,191);(787,191)} 
    dicmdCreateObjPolygon -points {(617,316);(795,307);(777,393);(728,411);(728,411)} 
    dicmdCreateObjLine -points {(486,87);(580,221)} 
    dicmdCreateObjLine -points {(566,327);(617,179)} 
    dicmdDesignSave -filename mydes.des 
    dicmdDesignLoad -filename mydes.des 
    dicmdSelectShapesByRegion -end (1027,469) -start (37,7) 

Expected results: 
    - Save should work
    - Load should bring to same state of design when saved
    - Selection comparision with golden should be as expected
