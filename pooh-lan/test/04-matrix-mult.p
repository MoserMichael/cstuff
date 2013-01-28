include 'matrix.inc'

a = [
       [ 1, 2, 3, 4 ],
       [ 5, 6, 7, 8 ],
       [ 9, 0, 1, 2 ],
       [ 3, 4, 5, 6 ]
    ]

c = make_identity_matrix( ~rows 4 ~cols 4 )
print_matrix( ~matrix c )

println( ~msg 'matrix a' )
print_matrix( ~matrix a )

matrix_scale( ~factor 2 ~matrix a )
println( ~msg 'scale a by 2' )
print_matrix( ~matrix a )

println( ~msg 'matrix b' )
b = make_upper_triangle( ~rows 4 ~cols 4 )
print_matrix( ~matrix b )

d = matrix_mult( ~leftm a  ~rightm b )
println( ~msg 'product of matrix a and b' )
print_matrix( ~matrix d )


