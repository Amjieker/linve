#include <bits/stdc++.h>

#include "../cookie.h"
using namespace std;
int main() {
  lins::Cookie cook;
  cook.parse(
      "uuid_tt_dd=10_21017226090-1636967641283-141613; UN=qq_51986723; "
      "Hm_up_6bcd52f51e9b3dce32bec4a3997715ac=%7B%22islogin%22%3A%7B%22value%"
      "22%3A%221%22%2C%22scope%22%3A1%7D%2C%22isonline%22%3A%7B%22value%22%3A%"
      "221%22%2C%22scope%22%3A1%7D%2C%22isvip%22%3A%7B%22value%22%3A%220%22%2C%"
      "22scope%22%3A1%7D%2C%22uid_%22%3A%7B%22value%22%3A%22qq_51986723%22%2C%"
      "22scope%22%3A1%7D%7D; "
      "Hm_ct_6bcd52f51e9b3dce32bec4a3997715ac=6525*1*10_21017226090-"
      "1636967641283-141613!5744*1*qq_51986723; "
      "Hm_lvt_6bcd52f51e9b3dce32bec4a3997715ac=1637853298,1637853835,"
      "1637853962,1637903300; UserName=qq_51986723; "
      "UserInfo=f36a91dc1aab4c75aa57df9f7b788e85; "
      "UserToken=f36a91dc1aab4c75aa57df9f7b788e85; UserNick=AMjieker; AU=B0F; "
      "BT=1652778123787; p_uid=U010000; "
      "ssxmod_itna2=eqIxgDyDB7i=G=3GHD8KR2Lxmx6D0Gjcl6a="
      "DnKfyDDsNFDLjr5u9bevNnlf5NDrL=uD3QyU5/Kl27deDoKjPTGWCt7GaG3dDWMnfKC/"
      "0IuLiuGdFqvgby=9cEehT6BnIbP4IZvMG25iC1yzDSerbPHTn=7720B+"
      "Uo5hH6w46CcC2OYroc4w2AiNhRqAt3WENG4Dw625ejxa="
      "hiq3Aq4gl8jWEa6ybiUDE0KDjKD+ZhDD===; "
      "ssxmod_itna=7qUxcDumD=GQe0LPYKBC7zeDtD7B=3FFDl=ODxA5D8D6DQeGTi6dzi=1="
      "K1kemli7o2DbkKj0W4qmYQAxEtrhQD4GLDmKDyinG3eDxPq0rD74irDDxD3GxneD+"
      "D0bSy0Tqi3DhxGQDpx0k86KDWgKDEm97DYvzDDy37x07DQH80hc+/"
      "ooDnhDCTQr8D75pDlpxI9r8tz8f1i5CsKShmQKGD40OD0IsExib8ooDU0I15+Tb="
      "jDe1GuKi0neYQ2KDmu41GDbiGiHK7=41j0x1Ni5vkDbTerPtMUDD=; c_adb=1; "
      "historyList-new=%5B%22%E4%BB%A3%E7%A0%81%E6%BA%90%E6%AF%8F%E6%97%A5%E4%"
      "B8%80%E9%A2%98%20div1%20%E6%95%B0%E6%95%B0%22%2C%22arc%20142%22%2C%"
      "22AtCoder%20Regular%20Contest%20142%22%2C%22AtCoder%20Beginner%"
      "20Contest%20256%22%2C%22Codeforces%20Round%20%23801%20(Div.%202)%22%5D; "
      "c_dl_prid=1657789478223_525127; c_dl_rid=1657941045418_667576; "
      "c_dl_fref=https://blog.csdn.net/weixin_30359021/article/details/"
      "97403285; c_dl_fpage=/download/exiscetti/5893067; "
      "c_dl_um=distribute.pc_relevant_t0.none-task-blog-2%7Edefault%7ECTRLIST%"
      "7Edefault-1-97403285-blog-125750395.pc_relevant_multi_platform_"
      "whitelistv2; c_first_ref=cn.bing.com; c_segment=13; "
      "dc_sid=bf7f90ed52a0e54ab4c1123a287a9528; "
      "dc_session_id=10_1659435571100.166808; csdn_highschool_close=close; "
      "c_edu_pm=edupm_new; log_Id_click=2176; "
      "c_pref=https%3A//edu.csdn.net/course/"
      "blogPlay%3Fgoods_id%3D14277%26blog_creator%3Dabcnull%26marketing_id%"
      "3D84%26lesson_id%3D280671%26spm%3D1002.2009.3001.4024; "
      "c_ref=https%3A//cn.bing.com/; "
      "c_first_page=https%3A//blog.csdn.net/talking12391239/article/details/"
      "9665185; c_dsid=11_1659441376881.192482; c_page_id=default; "
      "dc_tos=rfzl5s; log_Id_pv=1250; log_Id_view=4685");
  cout << cook.getString() << endl;
  return 0;
}