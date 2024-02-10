import requests
import sys

from requests.api import get

urls={
    'pll':'http://cubiclealgdbserver.azurewebsites.net/user/content/caseGroup/333/pll',
    'oll':'http://cubiclealgdbserver.azurewebsites.net/user/content/caseGroup/333/oll'
}

header={
    'User-Agent':'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/76.0.3809.100 Safari/537.36'
}

def get_algs(case_type):
    url=urls[case_type]
    response=requests.get(url,headers=header)
    print(response.text)
    
if __name__=='__main__':
    get_algs(sys.argv[2])