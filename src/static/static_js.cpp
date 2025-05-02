#include "static_js.h"

String staticJS_updates()
{
    String js = "";
    js += "<script>";
    js += "function o(e){return document.getElementById(e)}function i(e,t){return e=e.replace('v',''),t=t.replace('v',''),e.startsWith(t+'-')?-1:t.startsWith(e+'-')?1:e.localeCompare(t,void 0,{numeric:!0,sensitivity:'case',caseFirst:'upper'})}function r(e){return e.browser_download_url.split('/').pop()}async function getLatestGithubRelease(e){var t,a=await(await fetch('https://api.github.com/repos/speeduino/AirBear/releases/latest')).json(),n=a.tag_name;if(1==i(o('latest_release_txt').innerHTML=n,e)){o('update_btn').disabled=!1;for(const s of a.assets)s.name.includes('littlefs')?(t='http://speeduino.com/fw/AirBear/'+n+'/'+r(s),o('newData_url').value=t):(t='http://speeduino.com/fw/AirBear/'+n+'/'+r(s),o('newFW_url').value=t)}}async function scanWifi(){var e=o('ssid');for(const a of(await(await fetch('/wifi')).json()).networks){var t=document.createElement('option');t.value=a.ssid,t.text=a.ssid,e.add(t)}}function toggleData(){var e=o('newData_url');e.disabled=!e.disabled}function a(e,t){e.innerHTML=t}function updateProgress(){setTimeout(async()=>{let e;try{var t=await fetch('/updateStatus');e=await t.json()}catch(e){updateProgress()}e&&(a(o('updateStatus'),e.updateStatus),a(o('updateComplete'),e.updateProgress),a(o('updateSize'),e.updateSize),t=Math.floor(e.updateProgress/e.updateSize*100),a(o('updatePercent'),t),console.log(t),98<=t?setTimeout(()=>{const e=window.location;e.href='/',e=e.href},7e3):updateProgress())},1100)}const e=new EventSource('/events');e.addEventListener('debug',function(e){console.log('[Debug] '+e.data)},!1);";
    js += "</script>";

    return js;
}