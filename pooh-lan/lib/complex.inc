

# make a complex number in polar coordinates

sub make_polar_complex( rad, theta )
   return {
	    # complex number mode - polar coordinates
	    'mode' : 'polar',

	    # radius
	    'rad' : rad,

	    # angular value
	    'thet' : theta,

	    'add'   : sub( num )
			r = this.torect()
			res = r.add( ~num num )
			return res.topolar()
		      end,

	    'sub'   : sub( num )
		        r = this.torect()
		        res = r.add( ~num num )
			return res.topolar()
		      end,		       

	     'mult' : sub( num )
		        r = this.torect()
		        res = r.mult( ~num num )
			return res.topolar()
		      end,

	     'div'  : sub( num )
		        r = this.torect()
		        res = r.div( ~num num )
			return res.topolar()
		      end,		       

	       # returns the absolute number of this value (length of vector)
             'absolute' : sub() 
			    r = this.torect()
	  		    return r.absolute()
			  end,


	     # convert to rectangular coordinate
	     'torect' : sub()
			 make_rect_complex( ~re this.rad * cos( ~num this.theta ) ~im this.rad * sin( ~num this.theta ) )
			end,
	    			
	     'topolar' : sub()
			    return this
			end,

    # print the number
	    'print' : sub() 
			print( ~msg 'rectangular-polar: radius [ this.rad ] theta [ this.theta ]' )
		      end


	  }
end		  


# make a complex number in rectangular coordinates
sub make_rect_complex( re, im )

    return { 
	     # complex numer mode - rectangular/Cartesian coordinates
	     'mode' : 'rectangular',

	     # real value
	     're' : re,

	     #imaginairy value; return new object that is the result  
	     'im' : im,

	      # add this to argument num and return the result; return new object that is the result 
             'add' : sub( num )
			if num.mode ne 'rectangular'
			    num = num.torect()
			end
			return make_rect_complex( ~re this.re + num.re  ~im this.im + num.im )
		     end,

	      # multiply this by argument num and return the result; return new object that is the result 
	      'mult' : sub( num )
			if num.mode ne 'rectangular'
			    num = num.torect()
			end
		        return make_rect_complex( ~re this.re * num.re - this.im * num.im ~im this.im * num.re + this.re * num.im )
		     end,

	      # print the number
	      'print' : sub() 
			    print( ~msg 'rectangular-complex: real [ this.re ] imaginary [this.im]' )
		      end,

	      # divide this by the argument num; return new object that is the result
	      'div'   : sub( num )
			    if num.mode ne 'rectangular'
				num = num.torect()
			    end

		            sq = this.re * num.re + this.im * num.im 
			    re = ( this.re * num.re + this.im * num.im) / sq
			    im = ( num.im * this.re - this.re * num.im) / sq

			    return make_rect_complex( ~re re ~im im )
			end,

	       # returns the absolute number of this value (length of vector)
	       'absolute' : sub() 
				return sqr( ~num 3 ) #this.re * this re + this.im * this im )
			    end,

	       # convert to rectangular coordinates - return this object.
	       'torect' : sub()
			    return this
	  		  end,					

	       # convert to polar coordinates
	       'topolar' : sub()
			     rad = sqr( ~num this.re * this.re + this.im * this.im )
			     thet = atan2( ~x this.im ~y this.re )

			     return make_polar_complex( ~rad rad ~theta thet )
			   end
	   }			    
end
