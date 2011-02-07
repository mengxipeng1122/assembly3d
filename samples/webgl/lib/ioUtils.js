/*
 * Copyright (c) 2011 Michael Nischt
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 *    1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 *    2. Altered source versions must be plainly marked as such, and must not
 *    be misrepresented as being the original software.
 *
 *    3. This notice may not be removed or altered from any source
 *    distribution.
 */

function loadBinary(url, user, password)
{
	var request = new XMLHttpRequest();
	request.open('GET', url, false, user, password);
	request.responseType='arraybuffer';
	request.send();

	if(!request.status == 200) // HTTP's "OK"
	{
		alert("could not load: " + url);
		return null;
	}

  var buffer;
  if(request.mozResponseArrayBuffer && request.mozResponseArrayBuffer instanceof ArrayBuffer)
  {
      buffer = request.mozResponseArrayBuffer; 
  }
  else if(request.response instanceof ArrayBuffer)
  {
      buffer = request.response;
  }
  else 
  {
    buffer = null;
	}
  return buffer;
}

function loadBinaryAsync(url, done, user, password)
{
    var request = new XMLHttpRequest();
    request.open('GET', url, true, user, password);
    request.responseType='arraybuffer';
    request.onerror=function(e)
    {
	    alert("could not load: " + url);
    }    
    request.onload=function(e)
    {
        var buffer;
        if(request.mozResponseArrayBuffer && request.mozResponseArrayBuffer instanceof ArrayBuffer)
        {
            buffer = request.mozResponseArrayBuffer; 
        }
        else if(request.response instanceof ArrayBuffer)
        {
            buffer = request.response;
        }
        else 
        {
	        buffer = null;
				}
        done(buffer);
    };
    request.send();
}


function loadText(url, user, password)
{
	var request = new XMLHttpRequest();
	request.open('GET', url, false, user, password);
	request.send();

	if(!request.status == 200) // HTTP's "OK"
	{
		alert("could not load: " + url);
		return null;
	}

	return request.responseText;
}

function loadTextAsync(url, done, user, password)
{
	var request = new XMLHttpRequest();
	request.open('GET', url, true, user, password);
	request.onerror = function(e)
	{
		alert("could not load: " + url);	
	}
	request.onload = function(e)
	{
		done(request.responseText);
	}
	request.send();
}
