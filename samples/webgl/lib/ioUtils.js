/*
 * Copyright (c) 2011 Michael Nischt
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the project's author nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
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
