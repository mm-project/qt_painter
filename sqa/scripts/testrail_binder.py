from testrail import *
import os
import argparse

print(os.environ['HOME'])

client = APIClient(os.environ.get('TESTRAILS_BASEURL'))
client.user = os.environ.get('TESTRAILS_USR')
client.password = os.environ.get('TESTRAILS_PSW')

#todo
def get_summary_section(fn):
    pass

#todos
def get_expected_out(fn):
    pass

#fix
def init_connection():
    pass
	
#todo
def sync_testinfo(suite_id):
    filepath = '../tests.lst'
    with open(filepath) as fp:
        for cnt, line in enumerate(fp):
            tst = line.rstrip()
            #print(tst)
            result = client.send_post(
                'add_case/'+suite_id,
                { 'title': line.rstrip(),
                  'custom_steps': 'fixme1',
                  'custom_expected': 'fixme2',
                }
            )
            print(result)

 
def create_new_run(project_id,name,suiteid ):
    result = client.send_post('add_run/'+project_id,{'include_all': 1, 'name':name, 'suite_id': suiteid })
    #result = client.send_post('add_run/'+project_id,{'include_all': 0, 'name':name, 'case_ids': [ 45, 46 ] })
    #print(result)
    print(result['id'])

def update_test_result(run_id,testname,status,comment):
    resp = client.send_get('get_tests/'+run_id)
    test_id=""
    for r in resp:
        if r['title'] == testname:
            test_id=r['case_id']
            break
    
    if test_id is not "":
        result = client.send_post(
         	'add_result_for_case/'+run_id+'/'+str(test_id),
            { 'status_id': status, 'comment': comment }
        )
        print(result)
    else:
        print("Error: <"+testname+"> test not found")
        


#main fixme

parser = argparse.ArgumentParser(description='Optional app description')
parser.add_argument('-action', type=str, help='Action to do')
parser.add_argument('--runid', type=str, help='run id')
parser.add_argument('--testid', type=str, help='test id')
parser.add_argument('--projectid', type=str, help='project id')
parser.add_argument('--resultid', type=str, help='test result id')
parser.add_argument('--testname', type=str, help='test name')
parser.add_argument('--runname', type=str, help='run name')
parser.add_argument('--suiteid', type=str, help='suite id')
#parser.add_argument('--tstlst', type=str, help='test suites to run')
args = parser.parse_args()

# fixme
#init_connection()
if args.action == "create_new_run":
    create_new_run(args.projectid,args.runname,args.suiteid)
    
elif args.action == "update_test_result":
    update_test_result(args.runid,args.testname,args.resultid,"")

elif args.action == "sync_testinfo":
    sync_testinfo(args.suiteid)

else:
    print("Error: unsupported action")

